#pragma once

#include <raylib.h>
#include <string>
#include <utility>
#include <vector>

extern std::vector <Rectangle> GridCollisionCoord;

class TileMap

{

private:
	std::string MapCoord;
	std::vector<std::pair<Vector2, Vector2>> MapGrid;

public:

	TileMap();
	~TileMap();
	static void LoadMap();
	void MakeMap();
	void DrawMap();
	
};