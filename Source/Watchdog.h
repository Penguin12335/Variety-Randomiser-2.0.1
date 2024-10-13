#pragma once
#include "Memory.h"
#include "Panel.h"
#include "Randomizer.h"
#include "Generate.h"

class Watchdog
{
public:
	Watchdog(float time) {
		terminate = false;
		sleepTime = time;
		_memory = std::make_shared<Memory>("witness64_d3d11.exe");
	};
	void start();
	void run();
	virtual void action() = 0;
	float sleepTime;
	bool terminate;
protected:
	template <class T> std::vector<T> ReadPanelData(int panel, int offset, size_t size) {
		return _memory->ReadPanelData<T>(panel, offset, size);
	}
	template <class T> T ReadPanelData(int panel, int offset) {
		return _memory->ReadPanelData<T>(panel, offset);
	}
	template <class T> std::vector<T> ReadArray(int panel, int offset, int size) {
		return _memory->ReadArray<T>(panel, offset, size);
	}
	template <class T> void WritePanelData(int panel, int offset, const std::vector<T>& data) {
		return _memory->WritePanelData<T>(panel, offset, data);
	}
	template <class T> void WriteArray(int panel, int offset, const std::vector<T>& data) {
		return _memory->WriteArray<T>(panel, offset, data, false);
	}
	template <class T> void WriteArray(int panel, int offset, const std::vector<T>& data, bool force) {
		return _memory->WriteArray<T>(panel, offset, data, force);
	}
	std::shared_ptr<Memory> _memory;
};

class KeepWatchdog : public Watchdog {
public:
	KeepWatchdog() : Watchdog(10) { }
	virtual void action();
};

class SymbolWatchdog : public Watchdog {
public:
	SymbolWatchdog(int id) : Watchdog(0.1f) {
		Panel panel(id);
		this->id = id;
		grid = backupGrid = panel._grid;
		width = static_cast<int>(grid.size());
		height = static_cast<int>(grid[0].size());
		pillarWidth = tracedLength = 0;
		complete = false;
		style = ReadPanelData<int>(id, STYLE_FLAGS);
		DIRECTIONS = { Point(0, 2), Point(0, -2), Point(2, 0), Point(-2, 0), Point(2, 2), Point(2, -2), Point(-2, -2), Point(-2, 2) };
		if (ReadPanelData<int>(id, REFLECTION_DATA))
			symmetryData = ReadArray<int>(id, REFLECTION_DATA, ReadPanelData<int>(id, NUM_DOTS));
		for (Endpoint& e : panel._endpoints) {
			exits.emplace_back(panel.xy_to_loc(e.GetX(), e.GetY()));
		}
		exitPoint = (width / 2 + 1) * (height / 2 + 1);
	}
	SymbolWatchdog(int id, int pillarWidth) : SymbolWatchdog(id) {
		this->pillarWidth = pillarWidth;
		if (pillarWidth > 0) exitPoint = (width / 2) * (height / 2 + 1);
	}
	virtual void action();
	void initPath();
	bool check(int x, int y);
	bool checkArrow(int x, int y, int symbol);
	bool checkMine(int x, int y, int symbol);
	bool checkHead(int x, int y, int symbol);
	void DebugLog(int i);
	bool checkMushroom(int x, int y, int symbol);
	bool checkGhost(int x, int y, int symbol);
	bool checkBar(int x, int y, int symbol);
	std::set<Point> get_region_points_fw(Point pos);
	std::vector<int> get_region_grid_patterns_fw(std::set<Point> points);
	bool checkAntitriangle(int x, int y, int symbol);
	int get(Point p);
	bool check_it_is_corner(Point pos);
	bool checkDart(int x, int y, int symbol);
	bool checkRain(int x, int y, int symbol);
	bool isSurrounded(Point pos, Point dir, int type);
	bool checkPointer(int x, int y, int symbol);
	bool checkDiamond(int x, int y, int symbol);
	bool checkDice(int x, int y, int symbol);
	bool checkBell(int x, int y, int symbol);
	bool checkTent(int x, int y, int symbol);
	bool checkCircle(int x, int y, int symbol);
	bool checkNewSymbolsF(int x, int y, int symbol);
	std::set<Point> get_region_for_watchdog(Point pos);
	std::set<int> get_symbols_in_region_for_watchdog(const std::set<Point>& region);
	bool checkArrowPillar(int x, int y);

	int id;
	std::vector<std::vector<int>> backupGrid;
	std::vector<std::vector<int>> grid;
	int width, height, pillarWidth;
	int tracedLength;
	bool complete;
	int style;
	std::vector<int> exits;
	int exitPoint;
	std::vector<int> symmetryData;
	std::vector<Point> DIRECTIONS;
	template<class T>
	T pick_random_fw(const std::set<T>& set);
};

class BridgeWatchdog : public Watchdog {
public:
	BridgeWatchdog(int id1, int id2) : Watchdog(1) {
		solLength1 = false;
		solLength2 = false;
		this->id1 = id1; this->id2 = id2;
	}
	virtual void action();
	bool checkTouch(int id);
	int id1, id2, solLength1, solLength2;
};

class TreehouseWatchdog : public Watchdog {
public:
	TreehouseWatchdog(int id) : Watchdog(1) { }
	virtual void action();
};

class JungleWatchdog : public Watchdog {
public:
	JungleWatchdog(int id, std::vector<int> correctSeq1, std::vector<int> correctSeq2) : Watchdog(0.5f) {
		this->id = id;
		int size = ReadPanelData<int>(id, NUM_DOTS);
		sizes = ReadArray<int>(id, DOT_FLAGS, ReadPanelData<int>(id, NUM_DOTS));
		this->correctSeq1 = correctSeq1;
		this->correctSeq2 = correctSeq2;
		state = false;
		tracedLength = 0;
		ptr1 = ReadPanelData<long>(id, DOT_SEQUENCE);
		ptr2 = ReadPanelData<long>(id, DOT_SEQUENCE_REFLECTION);
	}
	virtual void action();
	int id;
	std::vector<int> sizes;
	long ptr1, ptr2;
	std::vector<int> correctSeq1, correctSeq2;
	bool state;
	int tracedLength;
};

class TownDoorWatchdog : public Watchdog {
public:
	TownDoorWatchdog() : Watchdog(0.2f) { }
	virtual void action();
};