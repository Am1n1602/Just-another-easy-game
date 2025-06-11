#pragma once

#include <raylib.h>
#include <cmath>


const int SCREENHEIGHT = 720;
const int SCREENWIDTH = 1280;

const Color TransParentGray=Fade(LIGHTGRAY, 0.6f);
const Color VibrantOrange = { 255, 140, 0,255 };
const Color BlueBlue = { 30, 144, 255,255 };
const Color BlueBlueTrans = Fade(BlueBlue, 0.6f);

enum class GameState

{
	MENU,
	PLAYING,
	PAUSE,
	GAMEOVER,
	LEADERBOARD

};

class MainGame

{

private:

	bool isLoaded;
	// Main-Menu objects
	void DrawMenu(Texture2D);
	void DrawPlaying();
	void DrawPause(Texture2D);
	void UpdateMenu(float deltaTime);
	void UpdatePlaying(float deltaTime);
	void UpdatePause(float deltaTime);
	void DrawGameOver(Texture2D);
	void UpdateGameOver();
	void DrawLeaderboard();
	void UpdateLeaderboard();

public:

	void FetchInBackground();
	GameState currentState;
	Camera2D camera;
	// Constructor and Destructor
	MainGame();
	~MainGame();
	void InitialCamera();
	static int DeathCount;
	Font fort;
	// Main Draw function

	static float TotalTime;
	void TimeTaken(float deltaTime,bool ShouldRun);
	void Draw();
	void Update(float deltaTime);
	void UpdateCamera(float deltaTime);
	bool GameShouldClose;
	void Unload();

};