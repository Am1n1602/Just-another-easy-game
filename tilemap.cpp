#include "tilemap.h"
#include <iostream>
// TODO <-Need to draw the map, rendering and drawing is implement ->

static Texture gameMap = { 0 };

TileMap::TileMap() : MapCoord("") {}
TileMap::~TileMap() {}


void TileMap::LoadMap()

{
	if (gameMap.id == 0) {
		gameMap = LoadTexture("assets/Tileset.png");
	}
}

void TileMap::MakeMap()

{
	this->MapCoord += "01";
	for (int i = 0; i < 38;i++) {
		this->MapCoord += "02";
	}
	this->MapCoord += "03";
}

void TileMap::DrawMap()

{
	int size = this->MapCoord.size();

	int whichTile1 = -1;
	int whichTile2 = -1;
	int whichTile = -1;
	int tileX = -1;
	int tileY = -1;
	int desX = -1;
	int desY = -1;

	for (int i = 0; i < (size)-1; i+=2)
	{
		if (this->MapCoord[i] == '0' && this->MapCoord[i + 1] == '0') {
			continue;
		}
		else {
			whichTile1 = this->MapCoord[i] - '0';
			whichTile2 = this->MapCoord[i+1] - '0';

			whichTile = ((10 * whichTile1) + whichTile2)-1;


			tileX = whichTile % 8;
			tileY = whichTile / 8;

			desX = (i / 2) % 40;
			desY = (i / 2) / 40;


			DrawTexturePro(gameMap, { (float)(tileX)*32,(float)(tileY)*32,32,32 }, { (float)desX * 32,(float)desY * 32,32,32 }, { 0,0 }, 0.0f, WHITE);

		}
	}
}

