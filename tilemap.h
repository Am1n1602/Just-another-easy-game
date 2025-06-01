#pragma once

#include <raylib.h>
#include <string>
#include <utility>
#include <vector>

extern std::vector<std::pair<Vector2, Vector2>> MapGrid;

class TileMap

{

private:
	std::string MapCoord;

public:

	TileMap();
	~TileMap();
	static void LoadMap();
	void MakeMap();
	void DrawMap();
	
};