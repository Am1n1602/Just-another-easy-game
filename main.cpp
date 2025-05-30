#include <raylib.h>
#include "game.h"

int main()

{
	// Initialization 

	InitWindow(SCREENWIDTH, SCREENHEIGHT, "My first game");
	SetTargetFPS(60);
	//ToggleBorderlessWindowed();


	//SetExitKey(KEY_P); Uncomment in the final testing

	// Initialise the game
	MainGame game;


	// Main game loop

	while (!WindowShouldClose())

	{
		// Update

		float deltaTime = GetFrameTime();

		game.Update(deltaTime);


		// Drawing on screen
		BeginDrawing();
		ClearBackground(BLACK);

		game.Draw();




		EndDrawing();


	}

	// De-Initialization
	CloseWindow();

	return 0;
}