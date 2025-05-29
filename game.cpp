#include "game.h"
#include "terrain.h"

MainGame::MainGame() : currentState(GameState::MENU),isLoaded(false) {}
MainGame::~MainGame(){}

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

void MainGame::DrawPlaying()

{
	Terrain::DrawBackground(isLoaded);

}

void MainGame::UpdatePlaying(float deltaTime)

{
	if (!isLoaded)
	{
		Terrain::LoadBackground(); // Load resources only once
		isLoaded = true;
	}
}

void MainGame::UpdateMenu(float deltaTime)

{
	if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
	
	{
		currentState = GameState::PLAYING;
	}
}

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


