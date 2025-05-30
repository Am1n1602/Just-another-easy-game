#include "terrain.h"
#include "game.h"



// give ID to initial background as 0
static Texture background[5] = {0};

// Load background only once

void Terrain::LoadBackground()

{
	if (background[0].id == 0) {
		background[0] = LoadTexture("assets/main_background.png");
	}

	if (background[1].id == 0) {
		background[1] = LoadTexture("assets/Overlay.png");
	}

	if (background[2].id == 0) {
		background[2] = LoadTexture("assets/background1.png");
	}

	if (background[3].id == 0) {
		background[3] = LoadTexture("assets/background2.png");
	}

	if (background[4].id == 0) {
		background[4] = LoadTexture("assets/background3.png");
	}
}


// Draw background every frame

/* in draw texture use a for loop from -1 to 1. and in DEST rectangle do {i*Screenwidth,i*Screenheight,Screenwidth, Screenheight}. */



void Terrain::DrawBackground()

{
	for (int i = 0;i < 5;i++)

	{

		if (background[0].id != 0) {

			DrawTexturePro(background[i], {0,0,576,324}, {0, 0,SCREENWIDTH,SCREENHEIGHT}, {0,0}, 0.0f, WHITE);
		}
	}

}








