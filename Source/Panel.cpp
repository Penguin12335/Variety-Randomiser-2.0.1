// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Panel.h"
#include "Special.h"
#include "Memory.h"
#include "Randomizer.h"
#include "Watchdog.h"
#include <sstream>
#include <fstream>

int Point::pillarWidth = 0;
std::vector<Panel> Panel::generatedPanels;
std::vector<std::tuple<int, int>> Panel::customSymbolPuzzles;

template <class T>
int find(const std::vector<T> &data, T search, size_t startIndex = 0) {
	for (size_t i=startIndex ; i<data.size(); i++) {
		if (data[i] == search) return static_cast<int>(i);
	}
	return -1;
}

Panel::Panel() {
	_memory = std::make_shared<Memory>("witness64_d3d11.exe");
}

Panel::Panel(int id) {
	_memory = std::make_shared<Memory>("witness64_d3d11.exe");
	Read(id);
}

void Panel::Read() {
	_width = 2 * _memory->ReadPanelData<int>(id, GRID_SIZE_X) - 1;
	if (_memory->ReadPanelData<int>(id, IS_CYLINDER)) {
		_width++;
		Point::pillarWidth = _width;
	}
	else Point::pillarWidth = 0;
	_height = 2 * _memory->ReadPanelData<int>(id, GRID_SIZE_Y) - 1;
	if (_width <= 0 || _height <= 0 || _width > 30 || _height > 30) {
		int numIntersections = _memory->ReadPanelData<int>(id, NUM_DOTS);
		_width = _height = static_cast<int>(std::round(sqrt(numIntersections))) * 2 - 1;
	}
	_grid.resize(_width);
	for (auto& row : _grid) row.resize(_height);
	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			_grid[x][y] = 0;
		}
	}
	_startpoints.clear();
	_endpoints.clear();

	_style = _memory->ReadPanelData<int>(id, STYLE_FLAGS);
	ReadAllData();
	ReadIntersections();
	ReadDecorations();
	pathWidth = 1;
	_resized = false;
	colorMode = ColorMode::Default;
	decorationsOnly = false;
	enableFlash = false;
}

void Panel::Write() {
	_memory->WritePanelData<int>(id, GRID_SIZE_X, { (_width + 1) / 2 });
	_memory->WritePanelData<int>(id, GRID_SIZE_Y, { (_height + 1) / 2 });
	if (_resized && _memory->ReadPanelData<int>(id, NUM_COLORED_REGIONS) > 0) {
		//Make two triangles that cover the whole panel
		std::vector<int> newRegions = { 0, xy_to_loc(_width - 1, 0), xy_to_loc(0, 0), 0, xy_to_loc(_width - 1, _height - 1), xy_to_loc(_width - 1, 0), 0, 0 };
		_memory->WritePanelData<int>(id, NUM_COLORED_REGIONS, { static_cast<int>(newRegions.size()) / 4 });
		_memory->WriteArray(id, COLORED_REGIONS, newRegions);
	}

	if (!decorationsOnly) WriteIntersections();
	else {
		std::vector<int> iflags = _memory->ReadArray<int>(id, DOT_FLAGS, _memory->ReadPanelData<int>(id, NUM_DOTS));
		for (int x = 0; x < _width; x += 2) {
			for (int y = 0; y < _height; y += 2) {
				if (_grid[x][y] & Decoration::Dot) {
					iflags[x / 2 + (y / 2) * (_width / 2 + 1)] = _grid[x][y];
					_style |= Style::HAS_DOTS;
				}
			}
		}
		_memory->WriteArray<int>(id, DOT_FLAGS, iflags);
	}
	WriteDecorations();
	if (enableFlash) _style &= ~NO_BLINK;
	_memory->WritePanelData<int>(id, STYLE_FLAGS, { _style });
	if (pathWidth != 1) _memory->WritePanelData<float>(id, PATH_WIDTH_SCALE, { pathWidth });
	_memory->WritePanelData<int>(id, NEEDS_REDRAW, { 1 });
	generatedPanels.push_back(*this);
}

void Panel::SetSymbol(int x, int y, Decoration::Shape symbol, Decoration::Color color)
{
	int gridx = x * 2 + (symbol & IntersectionFlags::COLUMN ? 0 : 1);
	int gridy = y * 2 + (symbol & IntersectionFlags::ROW ? 0 : 1);
	if (symbol & IntersectionFlags::DOT) {
		if (color == Decoration::Color::Blue || color == Decoration::Color::Cyan)
			color = static_cast<Decoration::Color>(IntersectionFlags::DOT_IS_BLUE);
		else if (color == Decoration::Color::Orange || color == Decoration::Color::Yellow)
			color = static_cast<Decoration::Color>(IntersectionFlags::DOT_IS_ORANGE);
		else color = Decoration::Color::None;
		if (symmetry) {
			Point sp = get_sym_point(gridx, gridy);
			SetGridSymbol(sp.first, sp.second, static_cast<Decoration::Shape>(symbol & ~Decoration::Dot), Decoration::Color::None);
		}
	}
	else if (symbol & IntersectionFlags::ROW || symbol & IntersectionFlags::COLUMN)
		color = Decoration::Color::None;
	SetGridSymbol(gridx, gridy, symbol, color);
}

void Panel::SetShape(int x, int y, int shape, bool rotate, bool negative, Decoration::Color color)
{
	if (!shape) return;
	int symbol = Decoration::Shape::Poly;
	while (!(shape & 0xf)) shape >>= 4;
	while (!(shape & 0x1111)) shape >>= 1;
	shape <<= 16;
	if (rotate) shape |= Decoration::Shape::Can_Rotate;
	else shape &= ~Decoration::Shape::Can_Rotate;
	if (negative) shape |= Decoration::Shape::Negative;
	else shape &= ~Decoration::Shape::Negative;
	_grid[x * 2 + 1][y * 2 + 1] = symbol | shape | color;
}

void Panel::ClearSymbol(int x, int y)
{
	ClearGridSymbol(x * 2 + 1, y * 2 + 1);
}

void Panel::SetGridSymbol(int x, int y, Decoration::Shape symbol, Decoration::Color color)
{
	if (symbol == Decoration::Start) _startpoints.push_back({ x, y });
	if (symbol == Decoration::Exit) {
		Endpoint::Direction dir;
		if (y == 0) dir = Endpoint::Direction::UP;
		else if (y == _height - 1) dir = Endpoint::Direction::DOWN;
		else if (x == 0) dir = Endpoint::Direction::LEFT;
		else dir = Endpoint::Direction::RIGHT;
		if (id == 0x033D4 || id == 0x0A3B5) {
			if (x == 0) dir = Endpoint::Direction::LEFT;
			else dir = Endpoint::Direction::RIGHT;
		}
		if (symmetry == Symmetry::ParallelH || symmetry == Symmetry::ParallelHFlip) {
			if (x == 0) dir = Endpoint::Direction::LEFT;
			if (x == _width - 1) dir = Endpoint::Direction::RIGHT;
		}
		_endpoints.emplace_back(Endpoint(x, y, dir, IntersectionFlags::ENDPOINT | 
			(dir == Endpoint::Direction::UP || dir == Endpoint::Direction::DOWN ?
				IntersectionFlags::COLUMN : IntersectionFlags::ROW)));
	}
	else _grid[x][y] = symbol | color;
}

void Panel::ClearGridSymbol(int x, int y)
{
	_grid[x][y] = 0;
}

void Panel::Resize(int width, int height)
{
	for (Point &s : _startpoints) {
		if (s.first == _width - 1) s.first = width - 1;
		if (s.second == _height - 1) s.second = height - 1;
	}
	for (Endpoint &e : _endpoints) {
		if (e.GetX() == _width - 1) e.SetX(width - 1);
		if (e.GetY() == _height - 1) e.SetY(height - 1);
	}
	if (_width != _height || width != height) {
		float maxDim = max(maxx - minx, maxy - miny);
		float unitSize = maxDim / max(width - 1, height - 1);
		minx = 0.5f - unitSize * (width - 1) / 2;
		maxx = 0.5f + unitSize * (width - 1) / 2;
		miny = 0.5f - unitSize * (height - 1) / 2;
		maxy = 0.5f + unitSize * (height - 1) / 2;
	}
	_width = width;
	_height = height;
	_grid.resize(width);
	for (auto& row : _grid) row.resize(height);
	_resized = true;
}

void Panel::ReadAllData() {
	Color pathColor = _memory->ReadPanelData<Color>(id, PATH_COLOR);
	Color rpathColor = _memory->ReadPanelData<Color>(id, REFLECTION_PATH_COLOR);
	Color successColor = _memory->ReadPanelData<Color>(id, SUCCESS_COLOR_A);
	Color strobeColor = _memory->ReadPanelData<Color>(id, STROBE_COLOR_A);
	Color errorColor = _memory->ReadPanelData<Color>(id, ERROR_COLOR);
	int numDecorations = _memory->ReadPanelData<int>(id, NUM_DECORATIONS);
	Color a = _memory->ReadPanelData<Color>(id, SYMBOL_A);
	Color b = _memory->ReadPanelData<Color>(id, SYMBOL_B);
	Color c = _memory->ReadPanelData<Color>(id, SYMBOL_C);
	Color d = _memory->ReadPanelData<Color>(id, SYMBOL_D);
	Color e = _memory->ReadPanelData<Color>(id, SYMBOL_E);
	Color ppColor = _memory->ReadPanelData<Color>(id, PATTERN_POINT_COLOR);
	Color ppColorA = _memory->ReadPanelData<Color>(id, PATTERN_POINT_COLOR_A);
	Color ppColorB = _memory->ReadPanelData<Color>(id, PATTERN_POINT_COLOR_B);
	int pushSymbolColors = _memory->ReadPanelData<int>(id, PUSH_SYMBOL_COLORS);
	int numColored = _memory->ReadPanelData<int>(id, NUM_COLORED_REGIONS);
	std::vector<int> colored = _memory->ReadArray<int>(id, COLORED_REGIONS, numColored * 4);
	int numConnections = _memory->ReadPanelData<int>(id, NUM_CONNECTIONS);
	int numDots = _memory->ReadPanelData<int>(id, NUM_DOTS);
	int reflectionData = _memory->ReadPanelData<int>(id, REFLECTION_DATA);
	std::vector<int> rdata; if (reflectionData) rdata = _memory->ReadArray<int>(id, REFLECTION_DATA, numDots);
	int style = _memory->ReadPanelData<int>(id, STYLE_FLAGS);
	std::vector<int> connections_a = _memory->ReadArray<int>(id, DOT_CONNECTION_A, numConnections);
	std::vector<int> connections_b = _memory->ReadArray<int>(id, DOT_CONNECTION_B, numConnections);
	int numIntersections = _memory->ReadPanelData<int>(id, NUM_DOTS);
	std::vector<float> intersections = _memory->ReadArray<float>(id, DOT_POSITIONS, numIntersections * 2);
	std::vector<int> intersectionFlags = _memory->ReadArray<int>(id, DOT_FLAGS, numIntersections);
	std::vector<int> decorations = _memory->ReadArray<int>(id, DECORATIONS, numDecorations);
	std::vector<int> decorationFlags = _memory->ReadArray<int>(id, DECORATION_FLAGS, numDecorations);
	float width = _memory->ReadPanelData<float>(id, PATH_WIDTH_SCALE);
	int seqLen = _memory->ReadPanelData<int>(id, SEQUENCE_LEN);
	std::vector<int> seq = _memory->ReadArray<int>(id, SEQUENCE, seqLen);
	std::vector<float> power = _memory->ReadPanelData<float>(id, POWER, 2);
	float openRate = _memory->ReadPanelData<float>(id, OPEN_RATE);
	int cptr = _memory->ReadPanelData<int>(id, DECORATION_COLORS);
	std::vector<Color> colors; if (cptr) colors = _memory->ReadArray<Color>(id, DECORATION_COLORS, numDecorations);
	Color outerBackground = _memory->ReadPanelData<Color>(id, OUTER_BACKGROUND);
	int outerBackgroundMode = _memory->ReadPanelData<int>(id, OUTER_BACKGROUND_MODE);
	Color bgRegionColor = _memory->ReadPanelData<Color>(id, BACKGROUND_REGION_COLOR);
	short metadata = _memory->ReadPanelData<short>(id, METADATA);
	//void* specularTexture = _memory->ReadPanelData<void*>(id, SPECULAR_TEXTURE);
	//std::vector<float> data = _memory->ReadPanelData<float>(id, SPECULAR_TEXTURE, 1000);
	int dotSeqLen = _memory->ReadPanelData<int>(id, DOT_SEQUENCE_LEN);
	std::vector<int> dotSeq = _memory->ReadArray<int>(id, DOT_SEQUENCE, dotSeqLen);
	int dotSeqLenR = _memory->ReadPanelData<int>(id, DOT_SEQUENCE_LEN_REFLECTION);
	std::vector<int> dotSeqR = _memory->ReadArray<int>(id, DOT_SEQUENCE_REFLECTION, dotSeqLenR);
	void* target = _memory->ReadPanelData<void*>(id, TARGET);
	void* panelTarget = _memory->ReadPanelData<void*>(id, PANEL_TARGET);
	Color cableTarget = _memory->ReadPanelData<Color>(id, CABLE_TARGET_2);
	//std::vector<int> targets = _memory->ReadArray<int>(id, PANEL_TARGET, 6);
	int isPillar = _memory->ReadPanelData<int>(id, IS_CYLINDER);
	int numTraced = _memory->ReadPanelData<int>(id, TRACED_EDGES);
	int numSol = _memory->ReadPanelData<int>(id, TRACED_EDGES + 4); //Don't know what this number is for yet
	int tracedptr = _memory->ReadPanelData<int>(id, TRACED_EDGE_DATA);
	//float solved = _memory->ReadPanelData<float>(id, PANEL_SOLVED);
	float distance = _memory->ReadPanelData<float>(id, MAX_BROADCAST_DISTANCE);
	std::vector<SolutionPoint> traced; if (tracedptr) traced = _memory->ReadArray<SolutionPoint>(id, TRACED_EDGE_DATA, numTraced);
}

void Panel::ReadDecorations() {
	int numDecorations = _memory->ReadPanelData<int>(id, NUM_DECORATIONS);
	std::vector<int> decorations = _memory->ReadArray<int>(id, DECORATIONS, numDecorations);
	std::vector<int> decorationFlags = _memory->ReadArray<int>(id, DECORATION_FLAGS, numDecorations);

	for (int i=0; i<numDecorations; i++) {
		auto [x, y] = dloc_to_xy(i);
		_grid[x][y] = decorations[i];
	}
}

void Panel::WriteDecorations() {
	std::vector<int> decorations;
	std::vector<Color> decorationColors;
	bool any = false;
	bool custom = false;
	_style &= ~0x3fc0; //Remove all element flags
	for (int y=_height-2; y>0; y-=2) {
		for (int x=1; x<_width; x+=2) {
			if (colorMode == ColorMode::Treehouse || colorMode == ColorMode::TreehouseAlternate) {
				if ((_grid[x][y] & 0xf) == Decoration::Color::Green) {
					_grid[x][y] &= ~0xf; _grid[x][y] |= 6;
				}
				if ((_grid[x][y] & 0xf) == Decoration::Color::Orange) {
					_grid[x][y] &= ~0xf; _grid[x][y] |= 5;
				}
				if ((_grid[x][y] & 0xf) == Decoration::Color::Magenta) {
					_grid[x][y] &= ~0xf; _grid[x][y] |= 4;
				}
			}
			decorations.push_back(_grid[x][y]);
			decorationColors.push_back(get_color_rgb(_grid[x][y] & 0xf));
			if (_grid[x][y])
				any = true;
			if ((_grid[x][y] & 0x700) == Decoration::Shape::Stone) _style |= HAS_STONES;
			else if ((_grid[x][y] & 0x700) == Decoration::Shape::Star) _style |= HAS_STARS;
			else if ((_grid[x][y] & 0x700) == Decoration::Shape::Poly) _style |= HAS_SHAPERS;
			else if ((_grid[x][y] & 0x700) == Decoration::Shape::Eraser) _style |= HAS_ERASERS;
			else if ((_grid[x][y] & 0x700) == Decoration::Shape::Triangle) _style |= HAS_TRIANGLES;
			else if (_grid[x][y] != 0) {
				_style |= HAS_TRIANGLES | HAS_STONES;
				custom = true;
			}
		}
	}
	if (!any) {
		_memory->WritePanelData<int>(id, NUM_DECORATIONS, { 0 });
	}
	else {
		_memory->WritePanelData<int>(id, NUM_DECORATIONS, { static_cast<int>(decorations.size()) });
		if (colorMode == ColorMode::WriteColors || colorMode == ColorMode::Treehouse || colorMode == ColorMode::TreehouseAlternate || _memory->ReadPanelData<int>(id, DECORATION_COLORS))
			_memory->WriteArray<Color>(id, DECORATION_COLORS, decorationColors);
		else if (colorMode == ColorMode::Reset || colorMode == ColorMode::Alternate) {
			_memory->WritePanelData<int>(id, PUSH_SYMBOL_COLORS, { colorMode == ColorMode::Reset ? 0 : 1 });
		}
		if (colorMode == ColorMode::Treehouse) {
			_memory->WritePanelData<int>(id, PUSH_SYMBOL_COLORS, { 1 });
			_memory->WritePanelData<Color>(id, SYMBOL_A, { { 0, 0, 0, 1 } }); //Black
			_memory->WritePanelData<Color>(id, SYMBOL_B, { { 1, 1, 1, 1 } }); //White
			_memory->WritePanelData<Color>(id, SYMBOL_C, { { 1, 0.5, 0, 1 } }); //Orange
			_memory->WritePanelData<Color>(id, SYMBOL_D, { { 1, 0, 1, 1 } }); //Magenta
			_memory->WritePanelData<Color>(id, SYMBOL_E, { { 0, 1, 0, 1 } }); //Green
		}
		else if (colorMode == ColorMode::TreehouseAlternate) {
			_memory->WritePanelData<int>(id, PUSH_SYMBOL_COLORS, { 1 });
			_memory->WritePanelData<Color>(id, SYMBOL_A, { { 0, 0, 0, 1 } }); //Black
			_memory->WritePanelData<Color>(id, SYMBOL_B, { { 0, 0, 1, 1 } }); //White->Blue
			_memory->WritePanelData<Color>(id, SYMBOL_C, { { 1, 0.5, 0, 1 } }); //Orange
			_memory->WritePanelData<Color>(id, SYMBOL_D, { { 1, 0, 1, 1 } }); //Magenta
			_memory->WritePanelData<Color>(id, SYMBOL_E, { { 1, 1, 1, 1 } }); //Green->White
		}
	}
	if (any || _memory->ReadPanelData<int>(id, DECORATIONS)) {
		_memory->WriteArray<int>(id, DECORATIONS, decorations);
		for (int i = 0; i < decorations.size(); i++) decorations[i] = 0;
		_memory->WriteArray<int>(id, DECORATION_FLAGS, decorations);
	}
	if (custom) {
		customSymbolPuzzles.emplace_back(id, Point::pillarWidth);
	}
}

void Panel::StartSymbolWatchdogs(const std::map<int, int>& shuffleMappings) {
	std::map<int, int> invertedMappings;
	for (const auto& [from, to] : shuffleMappings) {
		invertedMappings[to] = from;
	}
	for (const auto& [id, pillarWidth] : customSymbolPuzzles) {
		int realId = id;
		if (invertedMappings.count(realId)) realId = invertedMappings.at(realId);

		SymbolWatchdog* watchdog = new SymbolWatchdog(realId, pillarWidth);
		watchdog->start();
	}
}

void Panel::ReadIntersections() {
	int numIntersections = _memory->ReadPanelData<int>(id, NUM_DOTS);
	std::vector<float> intersections = _memory->ReadArray<float>(id, DOT_POSITIONS, numIntersections * 2);
	int num_grid_points = this->get_num_grid_points();
	minx = intersections[0]; miny = intersections[1];
	maxx = intersections[num_grid_points * 2 - 2]; maxy = intersections[num_grid_points * 2 - 1];
	if (minx > maxx) std::swap(minx, maxx);
	if (miny > maxy) std::swap(miny, maxy);
	unitWidth = (maxx - minx) / (_width - 1);
	if (Point::pillarWidth) unitWidth = 1.0f / _width;
	unitHeight = (maxy - miny) / (_height - 1);
	std::vector<int> intersectionFlags = _memory->ReadArray<int>(id, DOT_FLAGS, numIntersections);
	std::vector<int> symmetryData = _memory->ReadPanelData<int>(id, REFLECTION_DATA) ? 
		_memory->ReadArray<int>(id, REFLECTION_DATA, numIntersections) : std::vector<int>();
	if (symmetryData.size() == 0) symmetry = Symmetry::None;
	else if (symmetryData[0] == num_grid_points - 1) symmetry = Symmetry::Rotational;
	else if (symmetryData[0] == _width / 2 && intersections[1] == intersections[3]) symmetry = Symmetry::Vertical;
	else symmetry = Symmetry::Horizontal;

	for (int i = 0; i < num_grid_points; i++) {
		int x = static_cast<int>(std::round((intersections[i * 2] - minx) / unitWidth));
		int y = _height - 1 - static_cast<int>(std::round((intersections[i * 2 + 1] - miny) / unitHeight));
		_grid[x][y] = intersectionFlags[i];
		if (intersectionFlags[i] & IntersectionFlags::STARTPOINT) {
			_startpoints.push_back({x, y});
		}
	}

	for (int y = 0; y < _height; y++) {
		for (int x = 0; x < _width; x++) {
			if (x % 2 == y % 2) continue;
			_grid[x][y] = OPEN;
		}
	}
	int numConnections = _memory->ReadPanelData<int>(id, NUM_CONNECTIONS);
	std::vector<int> connections_a = _memory->ReadArray<int>(id, DOT_CONNECTION_A, numConnections);
	std::vector<int> connections_b = _memory->ReadArray<int>(id, DOT_CONNECTION_B, numConnections);
	//Remove non-existent connections
	std::vector<std::string> out;
	for (int i = 0; i < connections_a.size(); i++) {
		out.push_back("(" + std::to_string(connections_a[i]) + ", " + std::to_string(connections_b[i]) + ")");
		if (connections_a[i] >= num_grid_points || connections_b[i] >= num_grid_points) continue;
		int x = static_cast<int>(std::round((intersections[connections_a[i] * 2] - minx) / unitWidth));
		int y = _height - 1 - static_cast<int>(std::round((intersections[connections_a[i] * 2 + 1] - miny) / unitHeight));
		int x2 = static_cast<int>(std::round((intersections[connections_b[i] * 2] - minx) / unitWidth));
		int y2 = _height - 1 - static_cast<int>(std::round((intersections[connections_b[i] * 2 + 1] - miny) / unitHeight));
		_grid[(x + x2) / 2][(y + y2) / 2] = 0;
	}

	// Iterate the remaining intersections (endpoints, dots, gaps)
	for (int i = num_grid_points; i < numIntersections; i++) {
		float xd = (intersections[i * 2] - minx) / unitWidth;
		float yd = (intersections[i * 2 + 1] - miny) / unitHeight;
		int x = std::clamp((int)std::round(xd), 0, _width - 1);
		int y = _height - 1 - std::clamp((int)std::round(yd), 0, _height - 1);
		if (intersectionFlags[i] & IntersectionFlags::GAP) {
			float xd2 = (intersections[i * 2 + 2] - minx) / unitWidth;
			float yd2 = (intersections[i * 2 + 3] - miny) / unitHeight;
			x = std::clamp((int)std::round((xd + xd2) / 2), 0, _width - 1);
			y = _height - 1 - std::clamp((int)std::round((yd + yd2) / 2), 0, _height - 1);
			bool fake = false;
			for (int j = 0; j < numConnections; j++) {
				if (connections_a[j] == i && connections_b[j] == i + 1 ||
					connections_a[j] == i + 1 && connections_b[j] == i) {
					//Fake symmetry wall
					fake = true;
					_grid[x][y] = 0;
					i++;
					break;
				}
			}
			if (fake) continue;
			i++;
		}
		if (intersectionFlags[i] & IntersectionFlags::ENDPOINT) {
			for (int j = 0; j < numConnections; j++) {
				int location = -1;
				if (connections_a[j] == i) location = connections_b[j];
				if (connections_b[j] == i) location = connections_a[j];
				if (location != -1) {
					Endpoint::Direction dir = Endpoint::Direction::NONE;
					if (intersections[2 * i] < intersections[2 * location]) { // Our (i) x coordinate is less than the target's (location)
						dir = (Endpoint::Direction)(dir | Endpoint::Direction::LEFT);
					}
					if (intersections[2 * i] > intersections[2 * location]) {
						dir = (Endpoint::Direction)(dir | Endpoint::Direction::RIGHT);
					}
					if (intersections[2 * i + 1] < intersections[2 * location + 1]) { // y coordinate is 0 (bottom) 1 (top), so this check is reversed.
						dir = (Endpoint::Direction)(dir | Endpoint::Direction::DOWN);
					}
					if (intersections[2 * i + 1] > intersections[2 * location + 1]) {
						dir = (Endpoint::Direction)(dir | Endpoint::Direction::UP);
					}
					xd = (intersections[location * 2] - minx) / unitWidth;
					yd = (intersections[location * 2 + 1] - miny) / unitHeight;
					x = std::clamp((int)std::round(xd), 0, _width - 1);
					y = _height - 1 - std::clamp((int)std::round(yd), 0, _height - 1);
					_endpoints.emplace_back(Endpoint(x, y, dir, intersectionFlags[i]));
					break;
				}
			}
		}

		else {
			_grid[x][y] = intersectionFlags[i];
			//if (intersectionFlags[i] & IntersectionFlags::STARTPOINT) { //Glitchy right now
			//	_startpoints.push_back({ x, y });
			//}
		}
	}	
}

void Panel::WriteIntersections() {
	std::vector<float> intersections;
	std::vector<int> intersectionFlags;
	std::vector<int> connections_a;
	std::vector<int> connections_b;
	std::vector<int> symmetryData;
	std::vector<int> polygons;

	unitWidth = (maxx - minx) / (_width - 1);
	if (Point::pillarWidth) unitWidth = 1.0f / _width;
	unitHeight = (maxy - miny) / (_height - 1);

	for (Point p : _startpoints) {
		_grid[p.first][p.second] |= STARTPOINT;
	}

	_style &= ~HAS_DOTS;

	for (int y = _height - 1; y >= 0; y -= 2) {
		for (int x = 0; x <_width; x += 2) {
			intersections.push_back(static_cast<float>(minx + x * unitWidth));
			intersections.push_back(static_cast<float>(miny + (_height - 1 - y) * unitHeight));
			if (_grid[x][y] & IntersectionFlags::NO_POINT) intersectionFlags.push_back(_grid[x][y]);
			else intersectionFlags.push_back(_grid[x][y] | IntersectionFlags::INTERSECTION);
			if (_grid[x][y] & DOT) {
				_style |= HAS_DOTS;
				if (_grid[x][y] & IntersectionFlags::DOT_IS_BLUE || _grid[x][y] & IntersectionFlags::DOT_IS_ORANGE)
					_style |= IS_2COLOR;
			}

			// Create connections for this intersection -- always write low -> high
			if (y > 0 && _grid[x][y - 1] != OPEN) {
				connections_a.push_back(xy_to_loc(x, y - 2));
				connections_b.push_back(xy_to_loc(x, y));
			}
			if (x > 0 && _grid[x - 1][y] != OPEN) {
				connections_a.push_back(xy_to_loc(x - 2, y));
				connections_b.push_back(xy_to_loc(x, y));
			}
			if (symmetry) {
				symmetryData.push_back(xy_to_loc(get_sym_point(x, y).first, get_sym_point(x, y).second));
			}
		}
		if (Point::pillarWidth) {
			connections_a.push_back(xy_to_loc(_width - 2, y));
			connections_b.push_back(xy_to_loc(0, y));
		}
	}

	std::vector<std::string> out;
	for (int i = 0; i < connections_a.size(); i++) {
		out.push_back(std::to_string(connections_a[i]) + " -> " + std::to_string(connections_b[i]));
	}

	if (symmetry) {
		//Rearrange exits to be in symmetric pairs
		for (int i = 0; i < _endpoints.size(); i += 2) {
			Point sp = get_sym_point(_endpoints[i].GetX(), _endpoints[i].GetY());
			for (int j = i + 1; j < _endpoints.size(); j++) {
				if (_endpoints[j].GetX() == sp.first && _endpoints[j].GetY() == sp.second) {
					std::swap(_endpoints[i + 1], _endpoints[j]);
					break;
				}
			}
		}
	}

	double endDist = Point::pillarWidth == 0 ? 0.05 : 0.03;

	for (int i = 0; i < _endpoints.size(); i++) {
		Endpoint endpoint = _endpoints[i];
		int x = endpoint.GetX(); int y = endpoint.GetY();
		if (x % 2 || y % 2) {
			break_segment(x, y, connections_a, connections_b, intersections, intersectionFlags);
			connections_a.push_back(static_cast<int>(intersectionFlags.size()) - 1); // Target to connect to
		}
		else {
			connections_a.push_back(xy_to_loc(endpoint.GetX(), endpoint.GetY())); // Target to connect to
		}
		connections_b.push_back(static_cast<int>(intersectionFlags.size()));  // This endpoint
		double xPos = minx + endpoint.GetX() * unitWidth;
		double yPos = miny + (_height - 1 - endpoint.GetY()) * unitHeight;
		if (endpoint.GetDir() & Endpoint::Direction::LEFT) {
			xPos -= endDist;
		}
		if (endpoint.GetDir() & Endpoint::Direction::RIGHT) {
			xPos += endDist;
		}
		if (endpoint.GetDir() & Endpoint::Direction::UP) {
			yPos += endDist;
		}
		if (endpoint.GetDir() & Endpoint::Direction::DOWN) {
			yPos -= endDist;
		}
		intersections.push_back(static_cast<float>(xPos));
		intersections.push_back(static_cast<float>(yPos));
		intersectionFlags.push_back(endpoint.GetFlags());
		if (symmetry) {
			Point sp = get_sym_point(endpoint.GetX(), endpoint.GetY());
			for (int j = 0; j < _endpoints.size(); j++) {
				if (_endpoints[j].GetX() == sp.first && _endpoints[j].GetY() == sp.second) {
					symmetryData.push_back(get_num_grid_points() + j);
					break;
				}
				if (j == _endpoints.size() - 1) symmetryData.push_back(get_num_grid_points() + i); //No exit matches up with it symmetrically
			}
		}
	}

	// Dots/Gaps
	for (int y = _height - 1; y >= 0; y--) {
		for (int x = 0; x < _width; x++) {
			if (x % 2 == y % 2) continue;
			if (_grid[x][y] == 0 || _grid[x][y] == OPEN) continue;
			if (_grid[x][y] & IntersectionFlags::DOT) {
				_style |= HAS_DOTS;
				if (_grid[x][y] & IntersectionFlags::DOT_IS_BLUE || _grid[x][y] & IntersectionFlags::DOT_IS_ORANGE)
					_style |= IS_2COLOR;
			}
			if (locate_segment(x, y, connections_a, connections_b) == -1)
				continue;
			if (_grid[x][y] & IntersectionFlags::GAP) {
				if (!break_segment_gap(x, y, connections_a, connections_b, intersections, intersectionFlags))
					continue;
				if (symmetry) {
					auto[sx, sy] = get_sym_point(x, y);
					break_segment_gap(sx, sy, connections_a, connections_b, intersections, intersectionFlags);
					symmetryData.push_back(static_cast<int>(intersectionFlags.size()) - 1);
					symmetryData.push_back(static_cast<int>(intersectionFlags.size()) - 2);
					symmetryData.push_back(static_cast<int>(intersectionFlags.size()) - 3);
					symmetryData.push_back(static_cast<int>(intersectionFlags.size()) - 4);
					if (x % 2 == 0 && get_sym_dir(Endpoint::Direction::UP, symmetry) == Endpoint::Direction::UP ||
						y % 2 == 0 && get_sym_dir(Endpoint::Direction::LEFT, symmetry) == Endpoint::Direction::LEFT || symmetry == Symmetry::FlipXY) {
						std::swap(symmetryData[symmetryData.size() - 1], symmetryData[symmetryData.size() - 2]);
						std::swap(symmetryData[symmetryData.size() - 3], symmetryData[symmetryData.size() - 4]);
					}
				}
			}
			else {
				if (_grid[x][y] == IntersectionFlags::COLUMN || _grid[x][y] == IntersectionFlags::ROW)
					continue;
				if (!break_segment(x, y, connections_a, connections_b, intersections, intersectionFlags))
					continue;
				if (symmetry) {
					auto[sx, sy] = get_sym_point(x, y);
					if (!break_segment(sx, sy, connections_a, connections_b, intersections, intersectionFlags))
						continue;
					symmetryData.push_back(static_cast<int>(intersectionFlags.size()) - 1);
					symmetryData.push_back(static_cast<int>(intersectionFlags.size()) - 2);
				}
			}
		}
	}

	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			std::string s = std::to_string(_grid[x][y]);
			const char* mbs = s.data();
			size_t ret;
			mbstowcs_s(&ret, nullptr, 0, mbs, s.length());
			std::wstring ws(ret, 0);
			mbstowcs_s(&ret, &ws[0], ret, mbs, s.length());
			ws.resize(ret - 1);
			OutputDebugStringW(ws.data());
			OutputDebugStringW(L",");
		}
	}


	//Arrows (if applicable)
	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0x700) == Decoration::Arrow)
				render_arrow(x, y, (_grid[x][y] & 0xf000) >> 12, (_grid[x][y] & 0xf0000) >> 16, intersections, intersectionFlags, polygons);
		}
	}

	//NewSymbols (if applicable)
	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Mine)
				render_mine(x, y, (_grid[x][y] & 0xf0000) >> 16 , intersections, intersectionFlags, polygons);
		}
	}

	//NewSymbols2 (if applicable)
	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Head)
				render_head(x, y, (_grid[x][y] & 0xf0000) >> 16, intersections, intersectionFlags, polygons);
		}
	}

	//NewSymbols3 (if applicable)
	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Mushroom)
				render_mushroom(x, y, (_grid[x][y] & 0xf0000) >> 16, intersections, intersectionFlags, polygons);
		}
	}

	//NewSymbols4 (if applicable)
	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Ghost)
				render_ghost(x, y, (_grid[x][y] & 0xff0000) >> 16, intersections, intersectionFlags, polygons);
		}
	}

	//NewSymbols5 (if applicable)
	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Bar)
				render_bar(x, y, (_grid[x][y] & 0xf0000) >> 16, intersections, intersectionFlags, polygons);
		}
	}

	//NewSymbols6 (if applicable)
	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Antitriangle)
				render_antitriangle(x, y, (_grid[x][y] & 0xf0000) >> 16, intersections, intersectionFlags, polygons);
		}
	}

	//NewSymbols7 (if applicable)
	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Dart)
				render_dart(x, y, (_grid[x][y] & 0xf0000) >> 16, (_grid[x][y] & 0x0f000) >> 12, intersections, intersectionFlags, polygons);
		}
	}

	//NewSymbols8 (if applicable)
	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Rain)
				render_rain(x, y, (_grid[x][y] & 0xf0000) >> 16, intersections, intersectionFlags, polygons);
		}
	}

	//NewSymbols9 (if applicable)
	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Pointer)
				render_pointer(x, y, (_grid[x][y] & 0xf0000) >> 16, intersections, intersectionFlags, polygons);
		}
	}

	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Diamond)
				render_diamond(x, y, (_grid[x][y] & 0xf0000) >> 16, intersections, intersectionFlags, polygons);
		}
	}

	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Dice)
				render_dice(x, y, (_grid[x][y] & 0xf0000) >> 16, intersections, intersectionFlags, polygons);
		}
	}

	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Bell)
				render_bell(x, y, (_grid[x][y] & 0xf0000) >> 16, intersections, intersectionFlags, polygons);
		}
	}

	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Tent)
				render_tent(x, y, intersections, intersectionFlags, polygons);
		}
	}

	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0xF000700) == Decoration::Circle)
				render_circle(x, y, intersections, intersectionFlags, polygons);
		}
	}

	//Symmetry Data
	if (id == 0x01D3F && symmetry == Symmetry::None || id == 0x00076 && symmetry == Symmetry::None) {
		_style &= ~Style::SYMMETRICAL;
		_memory->WritePanelData<long long>(id, REFLECTION_DATA, { 0 });
	}
	else if (symmetryData.size() > 0) {
		_style |= Style::SYMMETRICAL;
		_memory->WriteArray<int>(id, REFLECTION_DATA, symmetryData);
	}
	else {
		_style &= ~Style::SYMMETRICAL;
		_memory->WritePanelData<long long>(id, REFLECTION_DATA, { 0 });
	}

	_memory->WritePanelData<int>(id, NUM_DOTS, { static_cast<int>(intersectionFlags.size()) });
	_memory->WriteArray<float>(id, DOT_POSITIONS, intersections);
	_memory->WriteArray<int>(id, DOT_FLAGS, intersectionFlags);
	_memory->WritePanelData<int>(id, NUM_CONNECTIONS, { static_cast<int>(connections_a.size()) });
	_memory->WriteArray<int>(id, DOT_CONNECTION_A, connections_a);
	_memory->WriteArray<int>(id, DOT_CONNECTION_B, connections_b);
	
	if (polygons.size() > 0) {
		_memory->WritePanelData<int>(id, NUM_COLORED_REGIONS, { static_cast<int>(polygons.size()) / 4 });
		_memory->WriteArray<int>(id, COLORED_REGIONS, polygons);
	}
}

void Panel::render_arrow(int x, int y, int ticks, int dir, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = { 0.1f, 0.45f, 0.1f, 0.55f, 0.85f, 0.45f, 0.85f, 0.55f,
		0.9f, 0.5f, 0.75f, 0.5f, 0.45f, 0.2f, 0.6f, 0.2f, 0.45f, 0.8f, 0.6f, 0.8f, };
	std::vector<int> polys = { 0, 1, 2, 0, 1, 2, 3, 0,
		4, 5, 7, 0, 5, 6, 7, 0, 4, 5, 9, 0, 5, 8, 9, 0, };
	if (ticks >= 2) {
		std::vector<float> positions2 = { 0.7f, 0.5f, 0.55f, 0.5f, 0.25f, 0.2f, 0.4f, 0.2f, 0.25f, 0.8f, 0.4f, 0.8f, };
		std::vector<int> polys2 = { 10, 11, 13, 0, 11, 12, 13, 0, 10, 11, 15, 0, 11, 14, 15, 0, };
		positions.insert(positions.end(), positions2.begin(), positions2.end());
		polys.insert(polys.end(), polys2.begin(), polys2.end());
	}
	if (ticks == 3) {
		std::vector<float> positions3 = { 0.5f, 0.5f, 0.35f, 0.5f, 0.05f, 0.2f, 0.2f, 0.2f, 0.05f, 0.8f, 0.2f, 0.8f, };
		std::vector<int> polys3 = { 16, 17, 19, 0, 17, 18, 19, 0, 16, 17, 21, 0, 17, 20, 21, 0, };
		positions.insert(positions.end(), positions3.begin(), positions3.end());
		polys.insert(polys.end(), polys3.begin(), polys3.end());
	}
	std::vector<int> angles = { -90, 90, 0, 180, -45, 45, 135, -135 };
	if (ticks == 3 && dir > 3) {
		for (int i = 0; i < positions.size(); i += 2) {
			positions[i] += 0.1f;
		}
	}
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y, angles[dir]);
}

void Panel::render_mine(int x, int y, int num, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = {};
	std::vector<int> polys = {};
	std::vector<float> positions_upper = { 0.35f, 0.8f, 0.4f, 0.85f, 0.6f, 0.85f, 0.65f, 0.8f, 0.6f, 0.75f, 0.4f, 0.75f };
	std::vector<float> positions_middle = { 0.35f, 0.5f, 0.4f, 0.55f, 0.6f, 0.55f, 0.65f, 0.5f, 0.6f, 0.45f, 0.4f, 0.45f };
	std::vector<float> positions_lower = { 0.35f, 0.2f, 0.4f, 0.25f, 0.6f, 0.25f, 0.65f, 0.2f, 0.6f, 0.15f, 0.4f, 0.15f };
	std::vector<float> positions_upper_left = { 0.35f, 0.8f, 0.4f, 0.75f, 0.4f, 0.55f, 0.35f, 0.5f, 0.3f, 0.55f, 0.3f, 0.75f };
	std::vector<float> positions_upper_right = { 0.65f, 0.8f, 0.7f, 0.75f, 0.7f, 0.55f, 0.65f, 0.5f, 0.6f, 0.55f, 0.6f, 0.75f };
	std::vector<float> positions_lower_left = { 0.35f, 0.5f, 0.4f, 0.45f, 0.4f, 0.25f, 0.35f, 0.2f, 0.3f, 0.25f, 0.3f, 0.45f };
	std::vector<float> positions_lower_right = { 0.65f, 0.5f, 0.7f, 0.45f, 0.7f, 0.25f, 0.65f, 0.2f, 0.6f, 0.25f, 0.6f, 0.45f };
	std::vector<int> polys_basic = { 0, 1, 2, 0, 0, 2, 3, 0, 0, 3, 4, 0, 0, 4, 5, 0, };
	std::vector<int> NUMINDICATE = {
		0b1011111,//0
		0b0000101,//1
		0b1110110,//2
		0b1110101,//3
		0b0101101,//4
		0b1111001,//5
		0b1111011,//6
		0b1001101,//7
		0b1111111,//8
		0b1111101,//9
	};
	std::vector<std::vector<float>> positions_list = { positions_upper,
		positions_middle,
		positions_lower,
		positions_upper_left,
		positions_upper_right,
		positions_lower_left,
		positions_lower_right,
	};

	for (int i = 0; i < 7; i++) {
		if ((NUMINDICATE[num] >> i) % 2 == 1) {
			positions.insert(positions.end(), positions_list[6 - i].begin(), positions_list[6 - i].end());
		};
	};
	for (int i = 0; i < positions.size() / 2; i += 6)
	{
		for (int j = 0; j < polys_basic.size(); j++) {
			if (j % 4 == 3) {
				polys.push_back(0);
			}
			else {
				polys.push_back(polys_basic[j] + i);
			}
		}
	}
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y);
};

void Panel::render_head(int x, int y, int num, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	if (num == 9) return;
	std::vector<float> positions = {
	0.5f,0.7f,
	0.45f, 0.9f,
	0.4f, 0.88f,
	0.35f, 0.85f,
	0.3f, 0.8f,
	0.25f, 0.72f,
	0.2f, 0.6f,
	0.15f, 0.55f,
	0.1f, 0.5f,
	0.2f, 0.51f,
	0.25f, 0.54f,
	0.275f, 0.4f,
	0.3f, 0.3f,
	0.35f, 0.25f,
	0.4f, 0.235f,
	0.45f, 0.22f,
	0.5f, 0.21f,
	0.55f, 0.22f,
	0.6f, 0.235f,
	0.65f, 0.25f,
	0.7f, 0.3f,
	0.725f, 0.4f,
	0.75f, 0.54f,
	0.8f, 0.51f,
	0.9f, 0.5f,
	0.85f, 0.55f,
	0.8f, 0.6f,
	0.75f, 0.72f,
	0.7f, 0.8f,
	0.65f, 0.85f,
	0.6f, 0.88f,
	0.55f, 0.9f,
	0.45f, 0.9f,
	};
	std::vector<int> polys_basic = { 0, 1, 2, 0 };
	std::vector<int> polys = {};
	std::vector<int> angles = { -90, 90, 0, 180, -45, 45, 135, -135 };

	for (int i = 0; i < positions.size() / 2; i += 1)
	{
		if (i + 2 >= positions.size() / 2) break;
		for (int j = 0; j < polys_basic.size(); j++) {
			if (j % 4 == 0 || j % 4 == 3) {
				polys.push_back(0);
			}
			else {
				polys.push_back(polys_basic[j] + i);
			}
		}
	}
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y, angles[num]);
};

void Panel::render_mushroom(int x, int y, int num, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = {
		0.5f,0.6f,
		0.5f, 0.8f,
		0.3f, 0.78f,
		0.2f, 0.7f,
		0.1f, 0.6f,
		0.16f, 0.53f,
		0.42f, 0.5f,
		0.4f, 0.2f,
		0.45f, 0.16f,
		0.5f, 0.15f,
		0.55f, 0.16f,
		0.6f, 0.2f,
		0.58f, 0.5f,
		0.84f, 0.53f,
		0.9f, 0.6f,
		0.8f, 0.7f,
		0.7f, 0.78f,
		0.5f, 0.8f,
	};
	std::vector<int> polys_basic = { 0, 1, 2, 0 };
	std::vector<int> polys = {};

	for (int i = 0; i < positions.size() / 2; i += 1)
	{
		if (i + 2 >= positions.size() / 2) break;
		for (int j = 0; j < polys_basic.size(); j++) {
			if (j % 4 == 0 || j % 4 == 3) {
				polys.push_back(0);
			}
			else {
				polys.push_back(polys_basic[j] + i);
			}
		}
	}
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y);
};

void Panel::render_ghost(int x, int y, int num, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = {
	0.5f, 0.9f,
	0.4f, 0.65f,
	0.4f, 0.88f,
	0.38f, 0.68f,
	0.3f, 0.8f,
	0.32f, 0.68f,
	0.25f, 0.7f,
	0.3f, 0.65f,
	0.225f, 0.65f,
	0.32f, 0.62f,
	0.2f, 0.6f,
	0.35f, 0.6f,
	0.1f, 0.2f,
	0.3f, 0.3f,
	0.35f, 0.6f,
	0.4f, 0.2f,
	0.35f, 0.6f,
	0.5f, 0.3f,
	0.38f, 0.62f,
	0.5f, 0.5f,
	0.4f, 0.65f,
	0.5f, 0.7f,
	0.5f, 0.9f,
	};

	int points_size = 23;

	std::vector<int> polys_basic = { 0, 1, 2, 0 };
	std::vector<int> polys = {};
	std::vector<float> positions_r = {};
	for (int i = 0; i < points_size * 2; i += 2) {
		positions_r.push_back(1 - positions[i]);
		positions_r.push_back(positions[i + 1]);
	};

	for (float r : positions_r) {
		positions.push_back(r);
	};

	for (int i = 0; i + 2 < points_size; i += 1)
	{
		for (int j = 0; j < polys_basic.size(); j++) {
			if (j % 4 == 3) {
				polys.push_back(0);
			}
			else {
				polys.push_back(polys_basic[j] + i);
			}
		}
	}

	for (int i = 0; i + 2 < points_size; i += 1)
	{
		for (int j = 0; j < polys_basic.size(); j++) {
			if (j % 4 == 3) {
				polys.push_back(0);
			}
			else {
				polys.push_back(polys_basic[j] + points_size + i);
			}
		}
	}
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y);
};

void Panel::render_bar(int x, int y, int num, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = { 0.4f, 0.4f, 0.4f, 0.6f, 0.6f, 0.6f, 0.6f, 0.4f };
	std::vector<int> polys = { 0, 1, 2, 0, 0, 2, 3, 0, };
	int point_count = 5;
	std::vector<float> positions_top = { 0.5f, 0.5f, 0.4f, 0.6f, 0.4f, 0.9f, 0.6f, 0.9f, 0.6f, 0.6f };
	std::vector<float> positions_right = { 0.5f, 0.5f, 0.6f, 0.6f, 0.9f, 0.6f, 0.9f, 0.4f, 0.6f, 0.4f };
	std::vector<float> positions_bottom = { 0.5f, 0.5f, 0.6f, 0.4f, 0.6f, 0.1f, 0.4f, 0.1f, 0.4f, 0.4f };
	std::vector<float> positions_left = { 0.5f, 0.5f, 0.4f, 0.4f, 0.1f, 0.4f, 0.1f, 0.6f, 0.4f, 0.6f };
	std::vector<int> polys_basic = { 4, 5, 6, 0, 4, 6, 7, 0, 4, 7, 8, 0, };
	//0:X(null) 1:(OOCC) 2:(COOC) 3:(CCOO) 4:(OCCO) 5:(COOO) 6:(OCOO) 7:(OOCO) 8:(OOOC) 9:(OOOO) A:(OCOC) B:(COCO) C:Gap_Column D:Gap_Row
	std::vector<int> NUMINDICATE = {
		0b0000,//0
		0b1100,//1
		0b0110,//2
		0b0011,//3
		0b1001,//4
		0b0111,//5
		0b1011,//6
		0b1101,//7
		0b1110,//8
		0b1111,//9
		0b1010,//A
		0b0101,//B
		0b1010,//CGap
		0b0101,//DGap
	};
	std::vector<std::vector<float>> positions_list = { positions_top,
		positions_right,
		positions_bottom,
		positions_left,
	};

	for (int i = 0; i < 4; i++) {
		if ((NUMINDICATE[num] >> i) % 2 == 1) {
			positions.insert(positions.end(), positions_list[positions_list.size() - 1 - i].begin(), positions_list[positions_list.size() - 1 - i].end());
		};
	};
	for (int i = 0; i < positions.size() / 2; i += point_count)
	{
		for (int j = 0; j < polys_basic.size(); j++) {
			if (j % 4 == 3) {
				polys.push_back(0);
			}
			else {
				polys.push_back(polys_basic[j] + i);
			}
		}
	}
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y);
};

void Panel::render_antitriangle(int x, int y, int num, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = {};
	std::vector<int> polys = {};
	int point_count = 3;
	float x_padding = 0;
	float y_padding = 0;
	std::vector<int> polys_basic = { 0, 1, 2, 0 };

	if (num == 1) {
		positions = { 0.5f, 0.3f,
			0.5f + 0.17320508f, 0.6f,
			0.5f - 0.17320508f, 0.6f };
	}
	else if (num == 2) {
		x_padding = -0.2f;
		std::vector<float> triangle_1 = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding };
		positions.insert(positions.end(), triangle_1.begin(), triangle_1.end());
		x_padding = 0.2f;
		std::vector<float> triangle_2 = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding };
		positions.insert(positions.end(), triangle_2.begin(), triangle_2.end());
	}
	else if (num == 3) {
		positions = { 0.5f + x_padding, 0.3f,
			0.5f + 0.17320508f, 0.6f,
			0.5f - 0.17320508f, 0.6f };
		x_padding = -0.4f;
		std::vector<float> triangle_B = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding };
		positions.insert(positions.end(), triangle_B.begin(), triangle_B.end());
		x_padding = 0.4f;
		std::vector<float> triangle_C = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding };
		positions.insert(positions.end(), triangle_C.begin(), triangle_C.end());
	}
	else if (num == 4) {
		x_padding = 0.2f;
		y_padding = 0.2f;
		std::vector<float> triangle_a = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding };
		positions.insert(positions.end(), triangle_a.begin(), triangle_a.end());
		x_padding = 0.2f;
		y_padding = -0.2f;
		std::vector<float> triangle_b = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding };
		positions.insert(positions.end(), triangle_b.begin(), triangle_b.end());
		x_padding = -0.2f;
		y_padding = 0.2f;
		std::vector<float> triangle_c = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding };
		positions.insert(positions.end(), triangle_c.begin(), triangle_c.end());
		x_padding = -0.2f;
		y_padding = -0.2f;
		std::vector<float> triangle_d = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding };
		positions.insert(positions.end(), triangle_d.begin(), triangle_d.end());
	}

	for (int i = 0; i < positions.size() / 2; i += point_count)
	{
		for (int j = 0; j < polys_basic.size(); j++) {
			if (j % 4 == 3) {
				polys.push_back(0);
			}
			else {
				polys.push_back(polys_basic[j] + i);
			}
		}
	}
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y);
};

void Panel::render_dart(int x, int y, int count, int dir, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = {};
	std::vector<int> polys = {};
	int point_count = 4;  //number of point
	float x_padding = 0;
	float y_padding = 0;
	std::vector<int> polys_basic = { 0, 1, 3, 0, 0, 2, 3, 0 };
	if (count == 1) {
		positions = { 0.5f, 0.3f,
			0.5f + 0.17320508f, 0.6f,
			0.5f - 0.17320508f, 0.6f,
			0.5f, 0.5f };
	}
	else if (count == 2) {
		x_padding = -0.2f;
		std::vector<float> triangle_1 = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding, 0.5f + y_padding };
		positions.insert(positions.end(), triangle_1.begin(), triangle_1.end());
		x_padding = 0.2f;
		std::vector<float> triangle_2 = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding, 0.5f + y_padding };
		positions.insert(positions.end(), triangle_2.begin(), triangle_2.end());
	}
	else if (count == 3) {
		positions = { 0.5f + x_padding, 0.3f,
			0.5f + 0.17320508f, 0.6f,
			0.5f - 0.17320508f, 0.6f,
			0.5f, 0.5f, };
		x_padding = -0.4f;
		std::vector<float> triangle_B = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding, 0.5f + y_padding };
		positions.insert(positions.end(), triangle_B.begin(), triangle_B.end());
		x_padding = 0.4f;
		std::vector<float> triangle_C = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding, 0.5f + y_padding };
		positions.insert(positions.end(), triangle_C.begin(), triangle_C.end());
	}
	else if (count == 4) {
		x_padding = 0.2f;
		y_padding = 0.2f;
		std::vector<float> triangle_a = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding, 0.5f + y_padding };
		positions.insert(positions.end(), triangle_a.begin(), triangle_a.end());
		x_padding = 0.2f;
		y_padding = -0.2f;
		std::vector<float> triangle_b = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding, 0.5f + y_padding };
		positions.insert(positions.end(), triangle_b.begin(), triangle_b.end());
		x_padding = -0.2f;
		y_padding = 0.2f;
		std::vector<float> triangle_c = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding, 0.5f + y_padding };
		positions.insert(positions.end(), triangle_c.begin(), triangle_c.end());
		x_padding = -0.2f;
		y_padding = -0.2f;
		std::vector<float> triangle_d = { 0.5f + x_padding, 0.3f + y_padding,
			0.5f + x_padding + 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding - 0.17320508f, 0.6f + y_padding,
			0.5f + x_padding, 0.5f + y_padding };
		positions.insert(positions.end(), triangle_d.begin(), triangle_d.end());
	}
	else {
		positions = { 0.2f, 0.3f,
		0.2f + 0.17320508f, 0.6f,
		0.2f - 0.17320508f, 0.6f,
		0.2f, 0.2f };
	}

	for (int i = 0; i < positions.size() / 2; i += point_count)
	{
		for (int j = 0; j < polys_basic.size(); j++) {
			if (j % 4 == 3) {
				polys.push_back(0);
			}
			else {
				polys.push_back(polys_basic[j] + i);
			}
		}
	}
	std::vector<int> angles = { 0, -180, 90, -90, 45, 135, -135, -45 };
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y, angles[dir]);
};

void Panel::render_rain(int x, int y, int dir, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = {
		0.5f,0.7f,
		0.5f, 0.9f,
		0.4f, 0.8f,
		0.35f, 0.7f,
		0.3f, 0.6f,
		0.25f, 0.5f,
		0.22f, 0.4f,
		0.23f, 0.3f,
		0.3f, 0.2f,
		0.4f, 0.15f,
		0.5f, 0.1f,
		0.6f, 0.15f,
		0.7f, 0.2f,
		0.77f, 0.3f,
		0.78f, 0.4f,
		0.75f, 0.5f,
		0.7f, 0.6f,
		0.65f, 0.7f,
		0.6f, 0.8f,
		0.5f, 0.9f,
	};
	std::vector<int> polys_basic = { 0, 1, 2, 0 };
	std::vector<int> polys = {};

	for (int i = 0; i < positions.size() / 2; i += 1)
	{
		if (i + 2 >= positions.size() / 2) break;
		for (int j = 0; j < polys_basic.size(); j++) {
			if (j % 4 == 0 || j % 4 == 3) {
				polys.push_back(0);
			}
			else {
				polys.push_back(polys_basic[j] + i);
			}
		}
	}

	std::vector<int> angles = { 0, 180, 90, -90, -45, 45, 135, -135 };
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y, angles[dir]);
};

void Panel::render_pointer(int x, int y, int type, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = {};
	std::vector<int> polys = {};
	std::vector<float> up = {
		0.5f, 0.9f,
		0.6f, 0.8f,
		0.4f, 0.8f,
		0.55f, 0.8f,
		0.45f, 0.8f,
		0.55f, 0.45f,
		0.45f, 0.45f,
	};
	std::vector<float> down = {
		0.5f, 0.1f,
		0.6f, 0.2f,
		0.4f, 0.2f,
		0.55f, 0.2f,
		0.45f, 0.2f,
		0.55f, 0.55f,
		0.45f, 0.55f,
	};
	std::vector<float> left = {
		0.1f, 0.5f,
		0.2f, 0.6f,
		0.2f, 0.4f,
		0.2f, 0.55f,
		0.2f, 0.45f,
		0.55f, 0.55f,
		0.55f, 0.45f,
	};
	std::vector<float> right = {
		0.9f, 0.5f,
		0.8f, 0.6f,
		0.8f, 0.4f,
		0.8f, 0.55f,
		0.8f, 0.45f,
		0.45f, 0.55f,
		0.45f, 0.45f,
	};
	std::vector<int> polys_basic = { 0, 1, 2, 0, 3, 4, 6, 0, 3, 5, 6, 0 };
	std::vector<std::vector<float>> positions_list = { up,down,left,right };

	for (int i = 0; i < 4; i++) {
		if ((type >> i) % 2 == 1) {
			positions.insert(positions.end(), positions_list[i].begin(), positions_list[i].end());
		};
	};
	for (int i = 0; i < positions.size() / 2; i += 7)
	{
		for (int j = 0; j < polys_basic.size(); j++) {
			if (j % 4 == 3) {
				polys.push_back(0);
			}
			else {
				polys.push_back(polys_basic[j] + i);
			}
		}
	}
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y);
};

void Panel::render_diamond(int x, int y, int num, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = { .5f, .05f, .95f, .5f, .5f, .95f, .05f, .5f,
		.5f, .2f, .8f, .5f, .5f, .8f, .2f, .5f, };
	std::vector<int> polys = { 0, 1, 4, 0, 1, 4, 5, 0, 1, 2, 5, 0, 2, 5, 6, 0,
		2, 3, 6, 0, 3, 6, 7, 0, 3, 4, 7, 0, 4, 0, 3, 0 };
	std::vector<float> positions2;
	std::vector<int> polys2;
	if (num == 2) {
		positions2 = { .5f, .4f, .4f, .5f, .5f, .6f, .6f, .5f };
		polys2 = { 8, 9, 10, 0, 10, 11, 8, 0 };
	}
	if (num == 3) {
		positions2 = { .45f, .1f, .55f, .1f, .55f, .9f, .45f, .9f };
		polys2 = { 8, 9, 10, 0, 10, 11, 8, 0 };
	}
	if (num == 4) {
		positions2 = { .45f, .9f, .55f, .9f, .55f, .45f, .45f, .45f,
			.5f, .55f, .5f, .45f, .3f, .4f, .3f, .3f, .7f, .4f, .7f, .3f, };
		polys2 = { 8, 9, 10, 0, 10, 11, 8, 0,
			 12, 13, 14, 0, 13, 14, 15, 0, 12, 13, 16, 0, 13, 16, 17, 0 };
	}
	if (num == 5) {
		positions2 = { .45f, .1f, .55f, .1f, .55f, .9f, .45f, .9f,
			.1f, .45f, .1f, .55f, .9f, .55f, .9f, .45f };
		polys2 = { 8, 9, 10, 0, 10, 11, 8, 0,
			12, 13, 14, 0, 14, 15, 12, 0 };
	}
	positions.insert(positions.end(), positions2.begin(), positions2.end());
	polys.insert(polys.end(), polys2.begin(), polys2.end());
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y);
};

void Panel::render_dice(int x, int y, int num, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = { .1f, .1f, .1f, .9f, .9f, .9f, .9f, .1f,
		.2f, .2f, .2f, .8f, .8f, .8f, .8f, .2f, };
	std::vector<int> polys = { 0, 1, 4, 0, 1, 4, 5, 0, 1, 2, 5, 0, 2, 5, 6, 0,
		2, 3, 6, 0, 3, 6, 7, 0, 3, 4, 7, 0, 4, 0, 3, 0 };
	std::vector<std::vector<int>> pips = { {4}, {2,6}, {2,4,6}, {0,2,6,8}, {0,2,4,6,8}, {0,2,3,5,6,8} };
	std::vector<float> positions2;
	std::vector<int> polys2;
	for (int i = 0; i < pips[num - 1].size(); i++) {
		float dx = (pips[num - 1][i] % 3) * .15f; float dy = (pips[num - 1][i] / 3) * .15f;
		positions2 = { .3f+dx, .3f+dy, .3f+dx, .4f+dy, .4f+dx, .4f+dy, .4f+dx, .3f+dy };
		polys2 = { 8 + i * 4, 9 + i * 4, 10 + i * 4, 0, 10 + i * 4, 11 + i * 4, 8 + i * 4, 0 };
		positions.insert(positions.end(), positions2.begin(), positions2.end());
		polys.insert(polys.end(), polys2.begin(), polys2.end());
	}
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y);
};

void Panel::render_bell(int x, int y, int dir, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = {
	0.5f, 0.8f,
	0.1f, 0.8f,
	0.2f, 0.7f,
	0.25f, 0.6f,
	0.25f, 0.4f,
	0.3f, 0.3f,
	0.4f, 0.2f,
	0.6f, 0.2f,
	0.7f, 0.3f,
	0.75f, 0.4f,
	0.75f, 0.6f,
	0.8f, 0.7f,
	0.9f, 0.8f,
	};
	std::vector<int> polys_basic = { 0, 1, 2, 0 };
	std::vector<int> polys = {};

	for (int i = 0; i < positions.size() / 2; i += 1)
	{
		if (i + 2 >= positions.size() / 2) break;
		for (int j = 0; j < polys_basic.size(); j++) {
			if (j % 4 == 0 || j % 4 == 3) {
				polys.push_back(0);
			}
			else {
				polys.push_back(polys_basic[j] + i);
			}
		}
	}
	std::vector<int> angles = { 180, 90, 0, -90 };
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y, angles[dir - 1]);
};

void Panel::render_tent(int x, int y, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = { .5f, .8f, .2f, .2f, .8f, .2f, .5f, .7f, .3f, .3f, .7f, .3f,
		.5f, .55f, .38f, .3f, .62f, .3f };
	std::vector<int> polys = { 0, 1, 3, 0, 1, 3, 4, 0, 1, 2, 4, 0, 2, 4, 5, 0, 2, 0, 5, 0, 0, 5, 3, 0, 6, 7, 8, 0};
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y);
};

void Panel::render_circle(int x, int y, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
	std::vector<float> positions = { .5f, .5f };
	std::vector<int> polys;
	int i = 0;
	for (int a = 0; a <= 360; a += 20) {
		positions.emplace_back(.5f + static_cast<float>(cos(a * 3.1415926536f / 180.0f)) * .4f);
		positions.emplace_back(.5f + static_cast<float>(sin(a * 3.1415926536f / 180.0f)) * .4f);
		if (a != 0) {
			polys.emplace_back(0); polys.emplace_back(i); polys.emplace_back(i + 1); polys.emplace_back(0);
		}
		i += 1;
	}
	//polys.emplace_back(0); polys.emplace_back(i); polys.emplace_back(1); polys.emplace_back(0);
	transform_and_place(positions, intersections, intersectionFlags, polys, polygons, x, y);
};

void Panel::render_newsymbolsF(int x, int y, int dir, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {

};