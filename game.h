#pragma once

#include <raylib.h>

const int SCREENHEIGHT = 720;
const int SCREENWIDTH = 1080;


enum class GameState

{
	MENU,
	PLAYING,
	PAUSE,
	GAMEOVER
};



class MainGame

{
private: 

	GameState currentState;

	// Main-Menu objects
	void DrawMenu();


public:

	// Constructor and Destructor
	MainGame();
	~MainGame();

	// Main Draw function
	void Draw();

};