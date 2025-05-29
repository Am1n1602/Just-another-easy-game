#include "terrain.h"
#include "game.h"

// give ID to initial background as 0
static Texture background = { 0 };

// Load background only once

void Terrain::LoadBackground()

{
	if (background.id==0) {
		background = LoadTexture("assets/background.png");
	}
}

// Draw background every frame

void Terrain::DrawBackground()

{
	if (background.id != 0) {

		DrawTexturePro(background, { 0,0,3400,2400 }, { 0, 0,SCREENWIDTH,SCREENHEIGHT }, { 0,0 }, 0.0f, WHITE);
	}
}


