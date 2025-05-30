#include "game.h"
#include "terrain.h"
#include "player.h"
#include "animation.h"

// Initial the global variables

MainGame::MainGame() : currentState(GameState::MENU),isLoaded(false) {}
MainGame::~MainGame(){}

//<- TODO: Add the player class and imply it ->//

Player player;
Animation PlayerAnim;


// Draw the main menu

void MainGame::DrawMenu()

{
	DrawRectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, SKYBLUE);

	const char* title = "My first Game";
	int titleWidth = MeasureText(title, 60);

	DrawText(title, SCREENWIDTH / 2 - titleWidth / 2, SCREENHEIGHT / 4, 60, DARKGREEN);
	const char* start = "Press SPACE to Start";
	int startWidth = MeasureText(start, 30);
	DrawText(start, SCREENWIDTH / 2 - startWidth / 2, 2*SCREENHEIGHT/ 3, 30, DARKGRAY);

}

// Draw the Playing Arena

void MainGame::DrawPlaying()

{
	Terrain::DrawBackground();
	player.DrawPlayer(PlayerAnim);
	

}

// Update the Playing area 

void MainGame::UpdatePlaying(float deltaTime)

{
	if (!isLoaded)
	{
		Terrain::LoadBackground(); // Load resources only once
		Player::LoadPlayer();
		isLoaded = true;
	}
	player.PlayerPositionUpdate(player.PlayerPosition);
	PlayerAnim.AnimationUpdate(deltaTime);

}

// Changes main menu to playing

void MainGame::UpdateMenu(float deltaTime)

{
	if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
	
	{
		currentState = GameState::PLAYING;
	}
}

// Main update function

void MainGame::Update(float deltaTime)

{
	switch (currentState)

	{
	case GameState::MENU:
		UpdateMenu(deltaTime);
		break;

	case GameState::PLAYING:
		UpdatePlaying(deltaTime);
		break;
	}
}

// Main draw function

void MainGame::Draw()

{
	switch (currentState)

	{
	case GameState::MENU:
		DrawMenu();
		break;

	case GameState::PLAYING:
		DrawPlaying();
		break;
	}
}


