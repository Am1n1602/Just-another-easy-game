#include "terrain.h"
#include "game.h"

// give ID to initial background as 0
static Texture background[1] = { 0 };

void Terrain::LoadBackground()
{
	if (background[0].id == 0) 
	{
		background[0] = LoadTexture("assets/DungeonBackground.png");
	}
}

void Terrain::DrawBackground(Player player)
{
	int x = player.PlayerPosition.x / SCREENWIDTH;
	int y = player.PlayerPosition.y / SCREENHEIGHT;
	for (int i = 0;i < 1;i++)
	{
		if (background[0].id != 0) {
			for (int k = -1 + y;k <= 1 + y;k++) {
				for (int j = -2 + x; j <= 2 + x;j++) {
					DrawTexturePro(background[i], { 0,0,(float)background[i].width,(float)background[i].height }, { (float)j * SCREENWIDTH, (float)k * SCREENHEIGHT,SCREENWIDTH,SCREENHEIGHT }, { 0,0 }, 0.0f, WHITE);
				}
			}
		}
	}
}

void Terrain::UnloadBackground()
{
	for (int i = 0; i < 1; i++)
	{
		UnloadTexture(background[i]);
	}
}