#pragma once


#include <raylib.h>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <deque>
#include <algorithm>
#include "objects.h"


extern std::vector <Rectangle> GridCollisionCoord;
extern std::vector <Rectangle> ObjectCollisionCoord;
extern std::deque <Rectangle> SavePointQueue;
extern std::deque <Rectangle> SavePointQueueBackup;

class TileMap

{

private:

	std::vector<std::pair<Rectangle, Rectangle>> MapGrid;


public:

	TileMap();
	~TileMap();
	void LoadMap();
	void DrawMap();
	void UnloadMap();
	//void DrawObjects();

};