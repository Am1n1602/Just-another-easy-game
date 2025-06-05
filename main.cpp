#include <raylib.h>
#include "game.h"


int main()

{
	// TODO IMP: Unload Texture to prevent Memory leak
	// Done (kinda, need to add more checks) 
	// TODO: Error checking

	// Initialization 
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "My first game");
	SetTargetFPS(60);
	InitAudioDevice();
	//ToggleFullscreen();
	//SetExitKey(KEY_P); Uncomment in the final testing
	// Initialise the game
	Music music = LoadMusicStream("assets/8-bit-dungeon-251388.mp3");
	PlayMusicStream(music);
	MainGame game;
	// Main game loop
	while (!WindowShouldClose())

	{
		// Update
		UpdateMusicStream(music);
		float deltaTime = GetFrameTime();
		game.Update(deltaTime);

		// Drawing on screen
		BeginDrawing();
		ClearBackground(BLACK);
		game.Draw();
		EndDrawing();

	}

	// De-Initialization
	UnloadMusicStream(music);
	game.Unload();
	CloseAudioDevice();
	CloseWindow();

	return 0;
}