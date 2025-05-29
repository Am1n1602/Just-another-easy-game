#include <raylib.h>
#include "game.h"

int main()
{
	// Initialization 

	InitWindow(SCREENWIDTH, SCREENHEIGHT, "Prototype");
	SetTargetFPS(60);

	// Main game loop

	while (!WindowShouldClose())
	{
		// Update

		float deltaTime = GetFrameTime();



		// Drawing on screen
		BeginDrawing();
		ClearBackground(SKYBLUE);

		EndDrawing();


	}

	// De-Initialization
	CloseWindow();

	return 0;
}