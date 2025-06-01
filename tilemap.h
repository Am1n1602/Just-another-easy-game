#pragma once

#include <raylib.h>
#include <string>



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