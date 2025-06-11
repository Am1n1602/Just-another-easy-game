
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
	InitAudioDevice();
	//ToggleFullscreen();

	Sound MainSound = LoadSound("assets/699618__seth_makes_sounds__164bpm-video-game-music-preview.wav");
	PlaySound(MainSound);
	bool exitWindowRequested = false;
	bool exitWindow = false;

	MainGame game;


	// Main game loop
	while (!exitWindow)

	{

		if (IsKeyPressed(KEY_F11) || IsKeyPressed(KEY_F) ||
			(IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))))
		{

			if (!(game.currentState == GameState::GAMEOVER)) {
				ToggleFullscreen();
			}
		}
		
		// Update
		if(!IsSoundPlaying(MainSound))
		{
			PlaySound(MainSound);
		}

		if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE) && (game.currentState== GameState::MENU))

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
				game.GameShouldClose = false;
				
			}
		}



		float deltaTime = GetFrameTime();
		if (game.currentState == GameState::PLAYING) {
			game.TimeTaken(deltaTime,true);
		}
		else if (game.currentState == GameState::MENU)
		{
			game.TimeTaken(deltaTime, false);
		}

		
		game.Update(deltaTime);
		if (game.GameShouldClose)
		{
			exitWindowRequested = true;
		}
		// Drawing on screen
		BeginDrawing();
		ClearBackground(BLACK);
		game.Draw();
		if (exitWindowRequested)

		{
			DrawRectangle(0, SCREENWIDTH/4,SCREENWIDTH,SCREENHEIGHT/4 , BLACK);
			DrawTextEx(game.fort,"Are you sure you want to exit the game? [Y/N]",{80,350},45,0.4,WHITE);
		}
		EndDrawing();

	}

	// De-Initialization
	UnloadSound(MainSound);
	game.Unload();
	CloseAudioDevice();
	CloseWindow();

	return 0;
}
