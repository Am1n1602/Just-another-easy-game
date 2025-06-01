#include "tilemap.h"


// Wtf is sauce (iykyk ;) )
std::vector<std::pair<Vector2, Vector2>> Sauce;
static Texture gameMap = { 0 };

TileMap::TileMap() : MapCoord("") {}
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
	this->MapCoord += "01";
	for (int i = 0; i < 38;i++)
	
	{
		this->MapCoord += "02";
	}

	this->MapCoord += "03";
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
			desX = (i / 2) % 40;
			desY = (i / 2) / 40;

			tile.x = tileX;
			tile.y = tileY;
			des.x = desX;
			des.y = desY;
			
			Sauce.push_back(std::make_pair(tile, des));
		}
	}

}

void TileMap::DrawMap()

{
	for (int i = 0;i < Sauce.size();i++)
	
	{
		DrawTexturePro(gameMap, { (float)(Sauce[i].first.x) * 32,(float)(Sauce[i].first.y) * 32,32,32}, {((float)Sauce[i].second.x) * 32,((float)Sauce[i].second.y) * 32,32,32}, {0,0}, 0.0f, WHITE);
	}
}

