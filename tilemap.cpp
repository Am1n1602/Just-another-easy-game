#include "tilemap.h"

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
	for (int i = 0;i < 20;i++) {
		this->MapCoord += "000000000000000000000000000000000000000";
	}
	this->MapCoord += "000000001222222222222222222222222222223";
}

void TileMap::DrawMap()

{
	int size = this->MapCoord.size();
	
	int whichTile = -1;
	int tileX = -1;
	int tileY = -1;
	int desX = -1;
	int desY = -1;

	for (int i = 0;i < size;i++) {
		if (this->MapCoord[i] == '0')
		{
			continue;
		}
		else {
			whichTile=this->MapCoord[i] - 1;
			tileX = whichTile % 8;
			tileY = whichTile / 8;
			desX = i % 40;
			desY = i / 40;


			DrawTexturePro(gameMap, { (float)tileX,(float)tileY,32,32 }, { (float)desX*32,(float)desY*32,32,32 }, { 0,0 }, 0.0f, WHITE);
		}
	}
}

