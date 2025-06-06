#pragma once

#include <raylib.h>


const int SCREENHEIGHT = 720;
const int SCREENWIDTH = 1280;

const Color TransParentGray=Fade(LIGHTGRAY, 0.6f);

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
	static int DeathCount;
	// Main Draw function
	void Draw();
	void Update(float deltaTime);
	void UpdateCamera(float deltaTime);

	void Unload();

};