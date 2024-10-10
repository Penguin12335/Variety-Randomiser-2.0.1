// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Watchdog.h"
#include "Quaternion.h"
#include <thread>
#include <iostream>
#include <stdlib.h>

void Watchdog::start()
{
	std::thread{ &Watchdog::run, this }.detach();
}

void Watchdog::run()
{
	while (!terminate) {
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTime * 1000)));
		action();
	}
}

//Keep Watchdog - Keep the big panel off until all panels are solved

void KeepWatchdog::action() {
	if (ReadPanelData<int>(0x01BE9, SOLVED)) {
		WritePanelData<float>(0x03317, POWER, { 1, 1 });
		WritePanelData<int>(0x03317, NEEDS_REDRAW, { 1 });
		terminate = true;
	}
	else {
		WritePanelData<float>(0x03317, POWER, { 0, 0 });
		WritePanelData<int>(0x03317, NEEDS_REDRAW, { 1 });
	}
}

//Symbol Watchdog - To run the custom symbol puzzles

void SymbolWatchdog::action() {
	int length = ReadPanelData<int>(id, TRACED_EDGES);
	if (length != tracedLength) {
		complete = false;
	}
	if (length == 0 || complete) {
		sleepTime = 0.1f;
		return;
	}
	sleepTime = 0.01f;
	if (length == tracedLength) return;
	initPath();
	if (complete) {
		for (int x = 1; x < width; x++) {
			for (int y = 1; y < height; y++) {
				if (!check(x, y)) {
					OutputDebugStringW(L"No");
					WritePanelData<int>(id, STYLE_FLAGS, { style | Panel::Style::HAS_TRIANGLES });
					return;
				}
			}
		}
		OutputDebugStringW(L"Yes");
		WritePanelData<int>(id, STYLE_FLAGS, { style & ~Panel::Style::HAS_TRIANGLES });
	}
}

void SymbolWatchdog::initPath()
{
	int numTraced = ReadPanelData<int>(id, TRACED_EDGES);
	int tracedptr = ReadPanelData<int>(id, TRACED_EDGE_DATA);
	if (!tracedptr) return;
	std::vector<SolutionPoint> traced = ReadArray<SolutionPoint>(id, TRACED_EDGE_DATA, numTraced);
	if (style & Panel::Style::SYMMETRICAL) {
		for (int i = 0; i < numTraced; i++) {
			SolutionPoint sp;
			if (traced[i].pointA >= exitPoint || traced[i].pointB >= exitPoint) {
				sp.pointA = sp.pointB = exitPoint;
			}
			else {
				sp.pointA = (width / 2 + 1) * (height / 2 + 1) - 1 - traced[i].pointA;
				sp.pointB = (width / 2 + 1) * (height / 2 + 1) - 1 - traced[i].pointB;
			}
			traced.push_back(sp);
		}
	}
	grid = backupGrid;
	tracedLength = numTraced;
	complete = false;
	if (traced.size() == 0) return;
	for (const SolutionPoint& p : traced) {
		int p1 = p.pointA, p2 = p.pointB;
		if (p1 == exitPoint || p2 == exitPoint) {
			complete = true;
			continue;
		}
		else if (p1 > exitPoint || p2 > exitPoint) continue;
		if (p1 == 0 && p2 == 0 || p1 < 0 || p2 < 0) {
			return;
		}
		int x1 = (p1 % (width / 2 + 1)) * 2, y1 = height - 1 - (p1 / (width / 2 + 1)) * 2;
		int x2 = (p2 % (width / 2 + 1)) * 2, y2 = height - 1 - (p2 / (width / 2 + 1)) * 2;
		if (pillarWidth > 0) {
			x1 = (p1 % (width / 2)) * 2, y1 = height - 1 - (p1 / (width / 2)) * 2;
			x2 = (p2 % (width / 2)) * 2, y2 = height - 1 - (p2 / (width / 2)) * 2;
			grid[x1][y1] = PATH;
			grid[x2][y2] = PATH;
			if (x1 == x2 || x1 == x2 + 2 || x1 == x2 - 2) grid[(x1 + x2) / 2][(y1 + y2) / 2] = PATH;
			else grid[width - 1][(y1 + y2) / 2] = PATH;
		}
		else {
			grid[x1][y1] = PATH;
			grid[x2][y2] = PATH;
			grid[(x1 + x2) / 2][(y1 + y2) / 2] = PATH;
		}
		if (p1 == exitPos || p2 == exitPos || (style & Panel::Style::SYMMETRICAL) && (p1 == exitPosSym || p2 == exitPosSym)) {
			complete = !complete;
		}
		else complete = false;
	}
}

bool SymbolWatchdog::check(int x, int y)
{
	int symbol = grid[x][y];
	int type = symbol & 0xF000700;
	if (type == Decoration::Triangle && (symbol & 0xf0000) != 0) {
		int count = 0;
		if (grid[x - 1][y] == PATH) count++;
		if (grid[x + 1][y] == PATH) count++;
		if (grid[x][y - 1] == PATH) count++;
		if (grid[x][y + 1] == PATH) count++;
		return count == (symbol >> 16);
	}
	if (type == Decoration::Arrow) {
		return checkArrow(x, y, symbol);
	}
	if (type == Decoration::Mine) {
		return checkMine(x, y, symbol);
	}
	if (type == Decoration::Head) {
		return checkHead(x, y, symbol);
	}
	if (type == Decoration::Mushroom) {
		return checkMushroom(x, y, symbol);
	}
	if (type == Decoration::Ghost) {
		return checkGhost(x, y, symbol);
	}
	if (type == Decoration::Bar) {
		return checkBar(x, y, symbol);
	}
	if (type == Decoration::Antitriangle) {
		return checkAntitriangle(x, y, symbol);
	}
	if (type == Decoration::Dart) {
		return checkDart(x, y, symbol);
	}
	if (type == Decoration::Rain) {
		return checkRain(x, y, symbol);
	}
	if (type == Decoration::Pointer) {
		return checkPointer(x, y, symbol);
	}
	if (type == Decoration::Diamond) {
		return checkDiamonds(x, y, symbol);
	}
	if (type == Decoration::Dice) {
		return checkDice(x, y, symbol);
	}
	if (type == Decoration::Bell) {
		return checkBells(x, y, symbol);
	}
	if (type == Decoration::NewSymbolsD) {
		return checkNewSymbolsD(x, y, symbol);
	}
	if (type == Decoration::NewSymbolsE) {
		return checkNewSymbolsE(x, y, symbol);
	}
	if (type == Decoration::NewSymbolsF) {
		return checkNewSymbolsF(x, y, symbol);
	}
	return true;
}

bool SymbolWatchdog::checkArrow(int x, int y, int symbol)
{
	if (pillarWidth > 0) return checkArrowPillar(x, y);
	int targetCount = (symbol & 0xf000) >> 12;
	Point dir = DIRECTIONS[(symbol & 0xf0000) >> 16];
	x += dir.first / 2; y += dir.second / 2;
	int count = 0;
	while (x >= 0 && x < width && y >= 0 && y < height) {
		if (grid[x][y] == PATH) {
			if (++count > targetCount)
				return false;
		}
		x += dir.first; y += dir.second;
	}
	return count == targetCount;
}

bool SymbolWatchdog::checkMine(int x, int y,int symbol) {
	Point pos = Point(x, y);
	std::set<Point> region = get_region_for_watchdog(Point(x, y));
	std::set<Point> nearby = {
			pos + Point(2,0), pos + Point(0,2), pos + Point(0,-2), pos + Point(-2,0),
			pos + Point(2,2), pos + Point(-2,2), pos + Point(-2,-2), pos + Point(2,-2),
	};
	int num = 0;
	for (Point a : nearby) {
		for (Point b : region) {
			if (a.first == b.first && a.second == b.second) {
				num += 1;
			}
		}
	}
	if ((pos.first == 1 || pos.first == width - 2) && (pos.second == 1 || pos.second == height - 2))
	{
		num = 3 - num;
	}
	else if ((pos.first == 1 || pos.first == width - 2) || (pos.second == 1 || pos.second == height - 2))
	{
		num = 5 - num;
	}
	else
	{
		num = 8 - num;
	}

	OutputDebugStringW(L"Test:");
	DebugLog(num);
	DebugLog(0xf0000 & symbol);
	return num == (0xf0000 & symbol) >> 16;
}

bool SymbolWatchdog::checkHead(int x, int y, int symbol) {
	if ((symbol & 0xf0000) >> 16 == 9) return true;
	std::vector<Point> _8dir = { Point(0, 2), Point(0, -2), Point(2, 0), Point(-2, 0), Point(2, 2), Point(2, -2), Point(-2, -2), Point(-2, 2) };
	Point dir = _8dir[(symbol & 0xf0000) >> 16];
	for (Point p : get_region_for_watchdog(Point(x, y))) {
		if (((dir.first == 0 || (p.first - x) * dir.first >  0) && (dir.second == 0 || (p.second - y) * dir.second > 0))) {
			if (!(grid[p.first][p.second] == 0x2090000 || grid[p.first][p.second]  == 0x600 || grid[p.first][p.second] == 0x0 || grid[p.first][p.second] == 0xA00)) return false;
		}
	}
	return true;
}

void SymbolWatchdog::DebugLog(int i) {
	std::string s = std::to_string(i);
	const char* mbs = s.data();
	size_t ret;
	mbstowcs_s(&ret, nullptr, 0, mbs, s.length());
	std::wstring ws(ret, 0);
	mbstowcs_s(&ret, &ws[0], ret, mbs, s.length());
	ws.resize(ret - 1);
	OutputDebugStringW(ws.data());
}

bool SymbolWatchdog::checkMushroom(int x, int y, int symbol) {
	std::vector<Point> DIRECTIONS = { Point(0, 2), Point(0, -2), Point(2, 0), Point(-2, 0) };
	for (Point dir : DIRECTIONS) {
		int local_x = x + dir.first / 2;
		int local_y = y + dir.second / 2;
		bool flag = false;
		while ((local_x >= 0 && local_x < width && local_y >= 0 && local_y < height) && !flag) {
			if (grid[local_x][local_y] == PATH) flag = true;
			local_x += dir.first; local_y += dir.second;
		}
		if (!flag) return false;
	}
	
	return true;
}

bool SymbolWatchdog::checkGhost(int x, int y, int symbol) {
	int num = 0;
	std::set<Point> open;
	for (int x = 1; x < width; x += 2) {
		for (int y = 1; y < height; y += 2) {
			open.emplace(Point(x, y));
		}
	}
	while (open.size() > 0) {
		Point pos = pick_random_fw(open);
		bool flag = false;
		for (Point p : get_region_for_watchdog(pos)) {
			if(flag && (get(p) & 0xf000000) == Decoration::Ghost)
			{
				OutputDebugStringW(L"(");
				DebugLog(p.first);
				OutputDebugStringW(L",");
				DebugLog(p.second);
				OutputDebugStringW(L")");
				OutputDebugStringW(L"too_many_ghost");
				return false;
			}
			else if ((get(p) & 0xf000000) == Decoration::Ghost) {

				OutputDebugStringW(L"(");
				DebugLog(p.first);
				OutputDebugStringW(L",");
				DebugLog(p.second);
				OutputDebugStringW(L")");
				flag = true;
			}
			open.erase(p);
		};
		if (!flag) {
			OutputDebugStringW(L"no_ghost");
			return false;
		}
		OutputDebugStringW(L"passed");
	}
	return true;
}

template <class T> T SymbolWatchdog::pick_random_fw(const std::set<T>& set) { auto it = set.begin(); std::advance(it, Random::rand() % set.size()); return *it; }

bool SymbolWatchdog::checkBar(int x, int y, int symbol) {
	int num = (symbol & 0xF0000) >> 16;
	Point pos = Point(x,y);
	//0:X(null) 1:┗(OOCC) 2:┏(COOC) 3:┓(CCOO) 4:┛(OCCO) 5:┳(COOO) 6:┫(OCOO) 7:┻(OOCO) 8:┣(OOOC) 9:╋(OOOO) A:┃(OCOC) B:━(COCO) C:Gap_Column D:Gap_Row
	std::vector<int> region_data = get_region_grid_patterns_fw(get_region_points_fw(pos));
	/*
	OutputDebugStringW(L"\nこの記号パターンとその座標:");
	DebugLog(num);
	OutputDebugStringW(L"(");
	DebugLog(x);
	OutputDebugStringW(L",");
	DebugLog(y);
	OutputDebugStringW(L")");
	OutputDebugStringW(L"\nこの領域にあった点の数:");
	DebugLog(get_region_points_fw(pos).size());
	OutputDebugStringW(L"\n領域パターン数(シンボルではない):(");
	for (int i = 0; i < 13; i++) {
		DebugLog(region_data[i]);
		OutputDebugStringW(L",");
	}
	OutputDebugStringW(L")");
	*/
	std::set<Point> symbols_data = get_region_for_watchdog(pos);
	for (Point p : symbols_data) {
		if ((grid[p.first][p.second] & 0xF0F0000) == (symbol & 0xF0F0000)) {
			region_data[(grid[p.first][p.second] & 0xF0000) >> 16] -= 1;
		}
	}
	return region_data[num] == 0;
}

std::set<Point> SymbolWatchdog::get_region_points_fw(Point pos) {
	std::set<Point> result;
	for (Point a : get_region_for_watchdog(pos)) {
		for (Point dir : { Point(0, 1), Point(0, -1), Point(1, 0), Point(-1, 0), Point(1, 1), Point(1, -1), Point(-1, -1), Point(-1, 1) }) {
			result.insert(a + dir);
		}
	}
	return result;
}

//0:X(null) 1:┗(OOCC) 2:┏(COOC) 3:┓(CCOO) 4:┛(OCCO) 5:┳(COOO) 6:┫(OCOO) 7:┻(OOCO) 8:┣(OOOC) 9:╋(OOOO) A:┃(OCOC) B:━(COCO) C:Gap_Column D:Gap_Row
std::vector<int> SymbolWatchdog::get_region_grid_patterns_fw(std::set<Point> points) {
	std::vector<int> result(14, 0);
	for (Point p : points) {
		if (p.first % 2 == 1 && p.second % 2 == 0 && grid[p.first][p.second] != PATH) {
			if (grid[p.first][p.second] == 0x300000 || grid[p.first][p.second] == 0x500000) {
				result[0xD] += 1;
			}
			else {
				result[0xB] += 1;
			}
			continue;
		}
		else if (p.first % 2 == 0 && p.second % 2 == 1 && grid[p.first][p.second] != PATH) {
			if (grid[p.first][p.second] == 0x300000 || grid[p.first][p.second] == 0x500000) {
				result[0xC] += 1;
			}
			else {
				result[0xA] += 1;
			}
			continue;
		}

		std::vector<bool> _4dir = { false,false,false,false };
		std::vector<std::vector<bool>> data = {
			{ true, true, false, false },
			{ false, true, true, false },
			{ false, false, true, true },
			{ true, false, false, true },
			{ false, true, true, true },
			{ true, false, true, true },
			{ true, true, false, true },
			{ true, true, true, false },
			{ true, true, true, true },
		};
		int count = 0;

		for (Point dir : {Point(0, -1), Point(1, 0), Point(0, 1), Point(-1, 0)}) {
			if ((p + dir).first < 0 || (p + dir).second < 0 || (p + dir).first >= width || (p + dir).second >= height || grid[p.first][p.second] == PATH) 
			{
				_4dir[count] = false;
			}
			else if (grid[(p + dir).first][(p + dir).second] != PATH) {
				_4dir[count] = true;
			}
			count++;
		}

		count = 0;
		for (std::vector<bool> d : data) {
			count++;
			if (_4dir == d) {
				result[count] += 1;
				/*
				OutputDebugStringW(L"(");
				DebugLog(p.first);
				OutputDebugStringW(L",");
				DebugLog(p.second);
				OutputDebugStringW(L")のパターンは");
				DebugLog(count);
				OutputDebugStringW(L"でした。\n");
				*/
			}
		}

	}
	return result;
}

bool SymbolWatchdog::checkAntitriangle(int x, int y, int symbol) {
	int num = 0;
	for (Point c : {Point(1, 1), Point(1, -1), Point(-1, -1), Point(-1, 1)}) {
		if (check_it_is_corner(Point(x + c.first,y + c.second))) {
			num += 1;
		}
	}

	return num == (symbol & 0xf0000) >> 16;
}

int SymbolWatchdog::get(Point p) { return grid[p.first][p.second]; }

bool SymbolWatchdog::check_it_is_corner(Point pos) {
	std::vector<bool> _4dir = { false,false,false,false };
	int i = 0;
	for (Point c : {Point(0, -1), Point(1, 0), Point(0, 1), Point(-1, 0)}) {
		if ((pos + c).first < 0 || (pos + c).second < 0 || (pos + c).first >= width || (pos + c).second >= height)
		{
			_4dir[i] = false;
		}
		else if (get(pos + c) == PATH) {
			_4dir[i] = true;
		}
		i++;
	}

	std::vector<std::vector<bool>> data = {
		{ true, true, false, false },
		{ false, true, true, false },
		{ false, false, true, true },
		{ true, false, false, true },
		{ false, true, true, true },
		{ true, false, true, true },
		{ true, true, false, true },
		{ true, true, true, false },
		{ true, true, true, true },
	};

	for (std::vector<bool> d : data) {
		if (_4dir == d)
		{
			return true;
		}
	}
	return false;
}

//
bool SymbolWatchdog::checkDart(int x, int y, int symbol) {
	int targetCount = (symbol & 0xf0000) >> 16;//the number
	Point dir = DIRECTIONS[(symbol & 0xf000) >> 12];//the direction
	int count = 0;
	std::set<Point> pointset = get_region_for_watchdog(Point(x, y));
	while (x >= 0 && x < width && y >= 0 && y < height) {
		x += dir.first; y += dir.second;
		for (Point p : pointset) {
			if (p == Point(x, y)) {
				count += 1;
			}
		}
	}

	DebugLog(count);
	DebugLog(targetCount);
	return count == targetCount;
}

bool SymbolWatchdog::checkRain(int x, int y, int symbol) {
	std::vector<Point> _8DIRECTIONS1 = { Point(0, 1), Point(0, -1), Point(1, 0), Point(-1, 0), Point(1, 1), Point(1, -1), Point(-1, -1), Point(-1, 1) };
	return isSurrounded(Point (x,y), _8DIRECTIONS1[(symbol & 0xf0000) >> 16], 2);
}

bool SymbolWatchdog::isSurrounded(Point pos, Point dir, int type) {
		std::vector<Point> spread;
		if (dir.first == 0) {
			spread = { {-1, 0} , {1, 0} };
		}
		else {
			spread = { {0, -1} , {0, 1} };
		}
		if (!(pos.first >= 0 && pos.first < width && pos.second >= 0 && pos.second < height)) {
			return false;
		}
		for (int i : {0, 1}) {
			if (get(pos + spread[i]) != PATH && (type == i || type == 2)) {
				if (!isSurrounded(pos + (spread[i] * 2), dir, i)) {
					return false;
				}
			}
		}

		if (get(pos + dir) != PATH) {
			if (!isSurrounded(pos + (dir * 2), dir, 2)) {
				return false;
			}
		}

		return true;
	}

bool SymbolWatchdog::checkPointer(int x, int y, int symbol) {
	int a = 0;
	std::vector<int> distance = { INT_MAX, INT_MAX , INT_MAX , INT_MAX };
	for (Point dir : {Point(2, 0), Point(-2, 0), Point(0, 2), Point(0, -2) }) {//DASW
		int count = 0;
		int x_tmp = x + dir.first / 2;
		int y_tmp = y + dir.second / 2;
		while (x_tmp >= 0 && x_tmp < width && y_tmp >= 0 && y_tmp < height && distance[a] == INT_MAX) {
			if (get(Point(x_tmp, y_tmp)) == PATH) {
				distance[a] = count;
			}
			x_tmp += dir.first; y_tmp += dir.second; count++;
		}
		a++;
	}
	std::vector<int> minbool = { 0, 0, 0, 0 };
	int min = INT_MAX;
	for (int v : distance) {
		if (v <= min) min = v;
	}
	if (min == INT_MAX) return false;
	a = 0;
	for (int v : distance) {
		if (v == min) {
			minbool[a] = 1;
		}
		a++;
	}

	int num = minbool[0] * 8 + minbool[1] * 4 + minbool[2] * 2 + minbool[3];

	if (num == 0) {
		num = 15;
	}
	return num == ((symbol & 0xf0000) >> 16);
}

bool SymbolWatchdog::checkDiamonds(int x, int y, int symbol) {
	int count = 0;
	std::set<Point> region = get_region_for_watchdog({ x, y });
	for (Point p : region) {
		if (get(p) != 0 && get(p) != Decoration::Triangle) {
			count++;
		}
	}
	return count == (symbol & 0xF0000) >> 16;
}

bool SymbolWatchdog::checkDice(int x, int y, int symbol) {
	int targetCount = 0;
	std::set<Point> region = get_region_for_watchdog({ x, y });
	for (Point p : region) {
		if ((get(p) & 0xF000000) == Decoration::Dice) {
			targetCount += (get(p) & 0xF0000);
		}
	}
	return region.size() == targetCount;
}

bool SymbolWatchdog::checkBells(int x, int y, int symbol) {
	return true;
}

bool SymbolWatchdog::checkNewSymbolsD(int x, int y, int symbol) {
	return true;
}

bool SymbolWatchdog::checkNewSymbolsE(int x, int y, int symbol) {
	return true;
}

bool SymbolWatchdog::checkNewSymbolsF(int x, int y, int symbol) {
	return true;
}

std::set<Point> SymbolWatchdog::get_region_for_watchdog(Point pos) {
	std::set<Point> region;
	std::vector<Point> check;
	check.push_back(pos);
	region.insert(pos);
	while (check.size() > 0) {
		Point p = check[check.size() - 1];
		check.pop_back();
		for (Point dir : { Point(0, 1), Point(0, -1), Point(1, 0), Point(-1, 0) }) {
			Point p1 = p + dir;
			if ((p1.first == 0 || p1.first + 1 == width) || p1.second == 0 || p1.second + 1 == height) continue;
			if (grid[p1.first][p1.second] == PATH || grid[p1.first][p1.second] == OPEN) continue;
			Point p2 = p + dir * 2;
			if ((grid[p2.first][p2.second] & Decoration::Empty) == Decoration::Empty) continue;
			if (region.insert(p2).second) {
				check.push_back(p2);
			}
		}
	}
	return region;
}

std::set<int> SymbolWatchdog::get_symbols_in_region_for_watchdog(const std::set<Point>& region) {
	std::set<int> symbols;
	for (Point p : region) {
		if (grid[p.first][p.second]) symbols.insert(grid[p.first][p.second]);
	}
	return symbols;
}

bool SymbolWatchdog::checkArrowPillar(int x, int y)
{
	int symbol = grid[x][y];
	if ((symbol & 0x700) == Decoration::Triangle && (symbol & 0xf0000) != 0) {
		int count = 0;
		if (grid[x - 1][y] == PATH) count++;
		if (grid[x + 1][y] == PATH) count++;
		if (grid[x][y - 1] == PATH) count++;
		if (grid[x][y + 1] == PATH) count++;
		return count == (symbol >> 16);
	}
	if ((symbol & 0x700) != Decoration::Arrow)
		return true;
	int targetCount = (symbol & 0xf000) >> 12;
	Point dir = DIRECTIONS[(symbol & 0xf0000) >> 16];
	x = (x + (dir.first > 2 ? -2 : dir.first) / 2 + pillarWidth) % pillarWidth; y += dir.second / 2;
	int count = 0;
	while (y >= 0 && y < height) {
		if (grid[x][y] == PATH) {
			if (++count > targetCount) return false;
		}
		x = (x + dir.first + pillarWidth) % pillarWidth; y += dir.second;
	}
	return count == targetCount;
}

void BridgeWatchdog::action()
{
	int length1 = _memory->ReadPanelData<int>(id1, TRACED_EDGES);
	int length2 = _memory->ReadPanelData<int>(id2, TRACED_EDGES);
	if (solLength1 > 0 && length1 == 0) {
		_memory->WritePanelData<int>(id2, STYLE_FLAGS, { _memory->ReadPanelData<int>(id2, STYLE_FLAGS) | Panel::Style::HAS_DOTS });
	}
	if (solLength2 > 0 && length2 == 0) {
		_memory->WritePanelData<int>(id1, STYLE_FLAGS, { _memory->ReadPanelData<int>(id1, STYLE_FLAGS) | Panel::Style::HAS_DOTS });
	}
	if (length1 != solLength1 && length1 > 0 && !checkTouch(id2)) {
		_memory->WritePanelData<int>(id2, STYLE_FLAGS, { _memory->ReadPanelData<int>(id2, STYLE_FLAGS) & ~Panel::Style::HAS_DOTS });
	}
	if (length2 != solLength2 && length2 > 0 && !checkTouch(id1)) {
		_memory->WritePanelData<int>(id1, STYLE_FLAGS, { _memory->ReadPanelData<int>(id1, STYLE_FLAGS) & ~Panel::Style::HAS_DOTS });
	}
	solLength1 = length1;
	solLength2 = length2;
}

bool BridgeWatchdog::checkTouch(int id)
{
	int length = _memory->ReadPanelData<int>(id, TRACED_EDGES);
	if (length == 0) return false;
	int numIntersections = _memory->ReadPanelData<int>(id, NUM_DOTS);
	std::vector<int> intersectionFlags = _memory->ReadArray<int>(id, DOT_FLAGS, numIntersections);
	std::vector<SolutionPoint> edges = _memory->ReadArray<SolutionPoint>(id, TRACED_EDGE_DATA, length);
	for (const SolutionPoint& sp : edges) if (intersectionFlags[sp.pointA] == Decoration::Dot_Intersection || intersectionFlags[sp.pointB] == Decoration::Dot_Intersection) return true;
	return false;
}

void TreehouseWatchdog::action()
{
	if (ReadPanelData<int>(0x03613, SOLVED)) {
		WritePanelData<float>(0x17DAE, POWER, { 1.0f, 1.0f });
		WritePanelData<int>(0x17DAE, NEEDS_REDRAW, { 1 });
		terminate = true;
	}
}

void JungleWatchdog::action()
{
	int numTraced = ReadPanelData<int>(id, TRACED_EDGES);
	if (numTraced == tracedLength) return;
	tracedLength = numTraced;
	int tracedptr = ReadPanelData<int>(id, TRACED_EDGE_DATA);
	if (!tracedptr) return;
	std::vector<SolutionPoint> traced = ReadArray<SolutionPoint>(id, TRACED_EDGE_DATA, numTraced);
	int seqIndex = 0;
	for (const SolutionPoint& p : traced) {
		if ((sizes[p.pointA] & IntersectionFlags::DOT) == 0) continue;
		if (sizes[p.pointA] & (0x1000 << (state ? correctSeq1[seqIndex] : correctSeq2[seqIndex])))
			seqIndex++;
		else return;
		if (seqIndex >= 1) {
			WritePanelData<long>(id, DOT_SEQUENCE, { state ? ptr1 : ptr2 } );
			WritePanelData<long>(id, DOT_SEQUENCE_REFLECTION, { state ? ptr2 : ptr1 });
			WritePanelData<int>(id, DOT_SEQUENCE_LEN, { state ? (int)correctSeq1.size() : (int)correctSeq2.size() });
			WritePanelData<int>(id, DOT_SEQUENCE_LEN_REFLECTION, { state ? (int)correctSeq2.size() : (int)correctSeq1.size() });
			state = !state;
			return;
		}
	}
}

void TownDoorWatchdog::action()
{
	if (ReadPanelData<Quaternion>(0x03BB0, ORIENTATION).z > 0) {
		WritePanelData<float>(0x28A69, POWER, { 1.0f, 1.0f });
		WritePanelData<int>(0x28A69, NEEDS_REDRAW, { 1 });
		terminate = true;
	}
}
