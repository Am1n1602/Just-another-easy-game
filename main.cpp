#include <raylib.h>
#include "game.h"
#include <iostream>

int main()

{
	// TODO IMP: Unload Texture to prevent Memory leak
	// Done (kinda, need to add more checks) 
	// TODO: Error checking

	// Initialization 
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "My first game");
	SetTargetFPS(60);
	SetExitKey(NULL);
	//InitAudioDevice();
	//ToggleFullscreen();
	//SetExitKey(KEY_P); Uncomment in the final testing
	// Initialise the game
	//Music music = LoadMusicStream("assets/8-bit-dungeon-251388.mp3");
	//PlayMusicStream(music);

	bool exitWindowRequested = false;
	bool exitWindow = false;

	MainGame game;



	// Main game loop
	while (!exitWindow)

	{
		// Update
		//UpdateMusicStream(music);

		if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE))

		{
			exitWindowRequested = true;
		}

		if (exitWindowRequested)

		{
			if (IsKeyPressed((KEY_Y)))

			{
				exitWindow = true;
			}
			else if (IsKeyPressed(KEY_N))

			{
				exitWindowRequested = false;
			}
		}

		float deltaTime = GetFrameTime();
		game.TimeTaken(deltaTime);
	
		game.Update(deltaTime);

		// Drawing on screen
		BeginDrawing();
		ClearBackground(BLACK);
		game.Draw();
		if (exitWindowRequested)

		{
			DrawRectangle(0, 200, SCREENWIDTH, 200, BLACK);
			DrawText("Are you sure you want to exit the game? [Y/N]", 80, 240, 40, WHITE);
		}
		EndDrawing();

	}

	// De-Initialization
	//UnloadMusicStream(music);
	game.Unload();
	//CloseAudioDevice();
	CloseWindow();

	return 0;
}