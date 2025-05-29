#pragma once

#include <raylib.h>

const int SCREENHEIGHT = 720;
const int SCREENWIDTH = 1080;

//<- TODO: Add the player class and imply it ->//

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
	bool isLoaded;

	// Main-Menu objects
	void DrawMenu();


	void DrawPlaying();

	void UpdateMenu(float deltaTime);

	void UpdatePlaying(float deltaTime);


public:

	// Constructor and Destructor
	MainGame();
	~MainGame();

	// Main Draw function
	void Draw();
	void Update(float deltaTime);

};