#include "tilemap.h"
#include <iostream>


static Texture gameMap = { 0 };

std::vector<Rectangle> GridCollisionCoord;

TileMap::TileMap() : MapCoord(""),MapGrid() {}
TileMap::~TileMap() {}

void TileMap::LoadMap()

{
	if (gameMap.id == 0)
	
	{
		gameMap = LoadTexture("assets/Tileset.png");
	}
}

void TileMap::MakeMap()

// TODO <-Make the actual map and try to optimise it->
{

	for (int i = 0;i < 21;i++) {
		for (int j = 0;j < 40;j++) {
			this->MapCoord += "00";
		}
	}
	for (int j = 0;j < 40;j++) {
		if (j > 21) {
			this->MapCoord += "01";
		}
		else {
			this->MapCoord += "00";
		}
	}

	this->MapCoord += "01";
	for (int i = 0; i < 38;i++)
	
	{
		if (i != 30 &&  i!=31) {
			this->MapCoord += "02";
		}
		else {
			this->MapCoord += "00";
		}
	}

	for (int i = 0;i < 24;i++) {
		for (int j = 0;j < 40;j++) {
			this->MapCoord += "00";
		}
	}



	this->MapCoord += "01";
	for (int i = 0; i < 38;i++)

	{
		if (i != 30 && i!=31) {
			this->MapCoord += "02";
		}
		else {
			this->MapCoord += "00";
		}
	}


	this->MapCoord += "63";
	int size = this->MapCoord.size();
	int whichTile1 = -1;
	int whichTile2 = -1;
	int whichTile = -1;
	int tileX = -1;
	int tileY = -1;
	int desX = -1;
	int desY = -1;
	Vector2 tile = { 0 };
	Vector2 des = { 0 };

	for (int i = 0; i < (size)-1; i+=2)

	{
		if (this->MapCoord[i] == '0' && this->MapCoord[i + 1] == '0')
		
		{
			continue;
		}

		else 
		
		{
			whichTile1 = this->MapCoord[i] - '0';
			whichTile2 = this->MapCoord[i+1] - '0';
			whichTile = ((10 * whichTile1) + whichTile2)-1;

			tileX = whichTile % 8;
			tileY = whichTile / 8;
			desX = ((i / 2) % 40) + (40*(i/2000));
			desY = ((i / 2) / 40) - (25*(i/2000));


			tile.x = tileX;
			tile.y = tileY;
			des.x = desX;
			des.y = desY;
			
			MapGrid.push_back(std::make_pair(tile, des));

			Rectangle x = { desX * 32, desY * 32, 32, 32 };
			//std::cout << x.x << " "<< x.y << " ";
			GridCollisionCoord.push_back(x);
		}
	}

}

void TileMap::DrawMap()

{
	for (int i = 0;i < MapGrid.size();i++)
	
	{
		DrawTexturePro(gameMap, { (float)(MapGrid[i].first.x) * 32,(float)(MapGrid[i].first.y) * 32,32,32}, {((float)MapGrid[i].second.x) * 32,((float)MapGrid[i].second.y) * 32,32,32}, {0,0}, 0.0f, WHITE);
	
	}
}

