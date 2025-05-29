#include "terrain.h"
#include "game.h"

static Texture background = { 0 };

void Terrain::LoadBackground()

{
	if (background.id==0) {
		background = LoadTexture("assets/background.png");
	}
}

void Terrain::DrawBackground(bool isBackgroundLoaded)

{
	if (background.id != 0) {

		DrawTexturePro(background, { 0,0,3400,2400 }, { 0, 0,SCREENWIDTH,SCREENHEIGHT }, { 0,0 }, 0.0f, WHITE);
	}
}


