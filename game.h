#pragma once

#include <raylib.h>


const int SCREENHEIGHT = 480;
const int SCREENWIDTH = 800;

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

	Camera2D camera;
	// Constructor and Destructor
	MainGame();
	~MainGame();
	void InitialCamera();
	// Main Draw function
	void Draw();
	void Update(float deltaTime);
	void UpdateCamera(float deltaTime);

	void Unload();

};