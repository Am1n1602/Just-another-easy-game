#include "game.h"


MainGame::MainGame() : currentState(GameState::MENU){}
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

void MainGame::Draw()

{
	switch (currentState)

	{
	case GameState::MENU:
		DrawMenu();
		break;
	}
}


