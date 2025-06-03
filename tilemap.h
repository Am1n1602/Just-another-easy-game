#pragma once


#include <raylib.h>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>




extern std::vector <Rectangle> GridCollisionCoord;

class TileMap

{

private:
	
	std::vector<std::pair<Rectangle, Rectangle>> MapGrid;
	

public:

	TileMap();
	~TileMap();
	 void LoadMap();
	/*void MakeMap();*/
	void DrawMap();
	
};