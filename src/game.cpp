#include "game.h"
#include "terrain.h"
#include "player.h"
#include "animation.h"
#include "tilemap.h"
#include "objects.h"
#include <iomanip>
// Initial the global variables


MainGame::MainGame() : currentState(GameState::MENU), isLoaded(false),
camera(),fort(LoadFont("assets/DungeonChunkMono.otf")),GameShouldClose(false) {
}
MainGame::~MainGame() {}

//<- TODO: Add the player class and imply it ->//
// <-Update 1: Partially done player class (idle,run,attack,jump) ->

//<--TODO: Add camera class and Background scrolling>
// <-Update 2: both done->

// <--TODO: Make Camera more smoother-->
// Done, Added Camera Smoothness

int MainGame::DeathCount = 0;
float MainGame::TotalTime = 0;
bool SwitchMenu = false;
bool PauseToResume = false;
bool PauseToExit = false;
bool MenuToExit = false;
bool isReset = false;
bool isWin = false;
int FinalMin = 0;
int FinalSec = 0;
int FinalMs = 0;
Sound ButtonSound;


Player player;
Animation playerAnim;
Animation object;
TileMap gameMap;
Objects allObjects;

Vector2 PlayerStartingPosition = player.PlayerPosition;

const float CAMERA_SMOOTHNESS = 0.04f;

void MainGame::InitialCamera()
{
	camera.offset.x = 0.0f; // One-third horizontally
	camera.offset.y = 0.0f; // Center vertically
	camera.target = { 0,0 };
	camera.rotation = 0.0f;
	camera.zoom = 2.2f; // 2.2f for final
}

// Draw the main menu

void MainGame::DrawMenu(Texture2D MainMenuBg)

{
	
	DrawTexturePro(MainMenuBg, { 0,0,(float)MainMenuBg.width,(float)MainMenuBg.height }, { 0,0,SCREENWIDTH,SCREENHEIGHT }, { 0,0 }, 0.0f, WHITE);

	DrawRectangleRounded({ 350,350,200,60 }, 0.2, 1, TransParentGray);
	if (!SwitchMenu) {
		DrawRectangleRoundedLinesEx({ 350,350,200,60 }, 0.3, 1, 2, WHITE);
		DrawTextPro(fort, "START GAME", { 353,360 }, { 0,0 }, 0.0f, 34, 2, WHITE);
	}
	else {
		DrawRectangleRoundedLinesEx({ 350,350,200,60 }, 0.3, 1, 2, BLUE);
		DrawTextPro(fort, "START GAME", { 353,360 }, { 0,0 }, 0.0f, 34, 2, SKYBLUE);
	}
	DrawRectangleRounded({ 650,350,200,60 }, 0.2, 1, TransParentGray);
	if (!MenuToExit) {
		DrawTextPro(fort, "QUIT GAME", { 660,360 }, { 0,0 }, 0.0f, 34, 2, WHITE);
		DrawRectangleRoundedLinesEx({ 650,350,200,60 }, 0.3, 1, 2, WHITE);
	}
	else {
		DrawTextPro(fort, "QUIT GAME", { 660,360 }, { 0,0 }, 0.0f, 34, 2, BLUE);
		DrawRectangleRoundedLinesEx({ 650,350,200,60 }, 0.3, 1, 2, SKYBLUE);
	}

	/*DrawRectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, SKYBLUE);

	const char* title = "My first Game";
	int titleWidth = MeasureText(title, 60);
	DrawText(title, SCREENWIDTH / 2 - titleWidth / 2, SCREENHEIGHT / 4, 60, DARKGREEN);

	const char* start = "Press SPACE to Start";
	int startWidth = MeasureText(start, 30);
	DrawText(start, SCREENWIDTH / 2 - startWidth / 2, 2 * SCREENHEIGHT / 3, 30, DARKGRAY);*/

}

// Draw the Playing Arena




void MainGame::UpdateCamera(float deltaTime)

{


	// One-third rule: player should be at 1/3 of the screen width
	float oneThirdX = SCREENWIDTH / 3.0f;
	float oneThirdY = SCREENHEIGHT / 2.0f; // Keep vertical center, 

	// Desired camera target so player appears at (oneThirdX, oneThirdY) on screen
	Vector2 desiredTarget{};
	desiredTarget.x = player.PlayerPosition.x - oneThirdX / camera.zoom;
	
	desiredTarget.y = player.PlayerPosition.y - oneThirdY / camera.zoom;

	// Smoothly interpolate camera.target towards desiredTarget
	camera.target.x += (desiredTarget.x - camera.target.x) * CAMERA_SMOOTHNESS;
	camera.target.y += (desiredTarget.y - camera.target.y) * CAMERA_SMOOTHNESS;


}


double globalTime = 0;

void MainGame::TimeTaken(float deltatime,bool ShouldRun)

{
	double elapsed = TotalTime += deltatime;
	globalTime = elapsed;
	if (!ShouldRun)
	{
		TotalTime = 0;
		DeathCount = 0;
	}
}


void MainGame::DrawPlaying()

{

	BeginMode2D(camera);
	Terrain::DrawBackground(player);
	gameMap.DrawMap();
	player.DrawPlayer(playerAnim);
	allObjects.DrawObjects(object,TYPEOBJECT::COIN);
	allObjects.DrawObjects(object, TYPEOBJECT::LAVA);

	// TODO: Either make a new class or function for this 
	// TODO: Make time in Min:sec:ms format
	// Done
	DrawRectangleRounded({ camera.target.x + 10,camera.target.y + 10,130,40 }, 0.2, 1, BlueBlueTrans);
	//DrawRectanglePro({ camera.target.x+10,camera.target.y+10,130,40 }, { 0,0 }, 0.0f, BlueBlue);
	DrawRectangleRoundedLinesEx({ camera.target.x + 10,camera.target.y + 10,130,40 }, 0.3, 1, 2, LIGHTGRAY);
	//DrawRectangleLinesEx({ camera.target.x + 10,camera.target.y + 10,130,40 }, 2.0f, VibrantOrange);
	Vector2 Temp = {camera.target.x + 20, camera.target.y + 20};
	Vector2 Temp2 = { camera.target.x + 20, camera.target.y + 35 };
	DrawTextEx(fort,TextFormat("DEATH COUNT: %d", DeathCount),Temp,10,2,YELLOW);
	int minutes = static_cast<int> (globalTime) / 60;
	FinalMin = minutes;
	int seconds = static_cast<int> (globalTime) % 60;
	FinalSec = seconds;
	int milliseconds = static_cast<int> (std::fmod(globalTime, 1.0) * 1000);
	FinalMs = milliseconds;
	DrawTextEx(fort, TextFormat("TIME: %02d:%02d:%02d", minutes,seconds,milliseconds), Temp2, 10, 2, YELLOW);

	
	
	
	EndMode2D();
	

}

void MainGame::DrawPause(Texture2D MainMenuBg)

{
	DrawTexturePro(MainMenuBg, { 0,0,(float)MainMenuBg.width,(float)MainMenuBg.height }, { 0,0,SCREENWIDTH,SCREENHEIGHT }, { 0,0 }, 0.0f, WHITE);
	DrawRectangleRounded({ 350,350,200,60 }, 0.2, 1, TransParentGray);
	if (!PauseToResume) {
		DrawRectangleRoundedLinesEx({ 350,350,200,60 }, 0.3, 1, 2, WHITE);
		DrawTextPro(fort, "RESUME", { 383,360 }, { 0,0 }, 0.0f, 34, 2, WHITE);
	}
	else {
		DrawRectangleRoundedLinesEx({ 350,350,200,60 }, 0.3, 1, 2, BLUE);
		DrawTextPro(fort, "RESUME", { 383,360 }, { 0,0 }, 0.0f, 34, 2, SKYBLUE);
	}
	DrawRectangleRounded({ 650,350,200,60 }, 0.2, 1, TransParentGray);
	if (!PauseToExit)
	{
	DrawTextPro(fort, "MAIN MENU", { 660,360 }, { 0,0 }, 0.0f, 34, 2, WHITE);
	DrawRectangleRoundedLinesEx({ 650,350,200,60 }, 0.3, 1, 2, WHITE);

	}
	else {
	DrawTextPro(fort, "MAIN MENU", { 660,360 }, { 0,0 }, 0.0f, 34, 2, BLUE);
	DrawRectangleRoundedLinesEx({ 650,350,200,60 }, 0.3, 1, 2, SKYBLUE);
	}

}

void MainGame::DrawGameOver(Texture2D MainMenuBg)

{
	DrawTexturePro(MainMenuBg, { 0,0,(float)MainMenuBg.width,(float)MainMenuBg.height }, { 0,0,SCREENWIDTH,SCREENHEIGHT }, { 0,0 }, 0.0f, WHITE);
	DrawRectangleRounded({ 0.3*SCREENWIDTH,0.44*SCREENHEIGHT,SCREENWIDTH/3,SCREENHEIGHT/5 }, 0.2, 1, VibrantOrange);
	DrawRectangleRoundedLinesEx({ 0.3 * SCREENWIDTH,0.44 * SCREENHEIGHT,SCREENWIDTH / 3,SCREENHEIGHT / 5 }, 0.3, 1, 2, WHITE);
	DrawTextPro(GetFontDefault(), "!!! Congratulations !!! ", {0.35 * SCREENWIDTH,0.46 * SCREENHEIGHT}, {0,0}, 0.0f, 34, 2, BLACK);
	DrawTextPro(GetFontDefault(), TextFormat("You took %d mins %d secs %d ms", FinalMin, FinalSec, FinalMs), { 0.317 * SCREENWIDTH, 0.52 * SCREENHEIGHT }, { 0,0 }, 0.0f, 27, 2, BLACK);
	DrawTextPro(GetFontDefault(), TextFormat("You died only %d times", DeathCount), { 0.323 * SCREENWIDTH, 0.57 * SCREENHEIGHT }, { 0,0 }, 0.0f, 27, 2, BLACK);
}

// Update the Playing area 

void MainGame::UpdatePlaying(float deltaTime)

{

	if (!isLoaded)

	{
		Terrain::LoadBackground(); // Load resources only once
		gameMap.LoadMap();
		Player::LoadPlayer();
		allObjects.LoadObjects();
		InitialCamera();
		player.LoadPlayerSound();

		isLoaded = true;
	}

	player.PlayerPositionUpdate(player.PlayerPosition,isReset,isWin);
	playerAnim.AnimationUpdate(deltaTime);
	object.AnimationUpdate(deltaTime);
	UpdateCamera(deltaTime);

	if (IsKeyPressed(KEY_ESCAPE))
	{
		currentState = GameState::PAUSE;
		PlaySound(ButtonSound);
	}
	if (isWin && currentState==GameState::PLAYING)
	{
		currentState = GameState::GAMEOVER;
	}
	// if (IsKeyPressed(KEY_ESCAPE))
	//{
	//	currentState = GameState::MENU;
	//} Uncomment in the final version 

}

// Changes main menu to playing

void MainGame::UpdateMenu(float deltaTime)

{
	Vector2 MousePos = GetMousePosition();

	if (CheckCollisionPointRec(MousePos, { 350,350,200,60 }))
	{
		
		SwitchMenu = true;
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			PlaySound(ButtonSound);
			currentState = GameState::PLAYING;
		}
	}
	else {
		SwitchMenu = false;
	}


	if (IsKeyPressed(KEY_ENTER))

	{
		PlaySound(ButtonSound);
		currentState = GameState::PLAYING;
	}

	if (CheckCollisionPointRec(MousePos, { 650,350,200,60 }))
	{

		MenuToExit = true;
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			currentState = GameState::MENU;
			GameShouldClose = true;
			PlaySound(ButtonSound);

		}
	}
	else {
		MenuToExit = false;
	}
	if (IsKeyPressed(KEY_ENTER))

	{
		currentState = GameState::MENU;
		PlaySound(ButtonSound);
	}

}

void MainGame::UpdatePause(float deltaTime)

{
	Vector2 MousePos = GetMousePosition();
	if (currentState == GameState::PAUSE)

	{
		
			if (CheckCollisionPointRec(MousePos, { 350,350,200,60 }))
			{

				PauseToResume = true;
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
					PlaySound(ButtonSound);
					currentState = GameState::PLAYING;
					
				}
			}
			else {
				PauseToResume = false;
			}
			if (IsKeyPressed(KEY_ESCAPE)) {
				PlaySound(ButtonSound);
				currentState = GameState::PLAYING;
				
			}

			if (CheckCollisionPointRec(MousePos, { 650,350,200,60 }))
			{

				PauseToExit = true;
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
					PlaySound(ButtonSound);
					currentState = GameState::MENU;
					isReset = true;
				}
			}
			else {
				PauseToExit = false;
			}

	}
}

void MainGame::UpdateGameOver()

{
	if (GetKeyPressed())
	{
		currentState = GameState::MENU;
		isReset = true;
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
	case GameState::GAMEOVER:
		UpdateGameOver();
		break;
	case GameState::PAUSE:
		UpdatePause(deltaTime);
		break;

	}

}

// Main draw function
bool isBgLoaded = false;
Texture2D MainMenuBg;

void MainGame::Draw()

{

	switch (currentState)

	{
	case GameState::MENU:
		if (!isBgLoaded)
		{
			MainMenuBg = LoadTexture("assets/Main_Menu_Bg2.png");
			ButtonSound = LoadSound("assets/615542__crash_358__sci-fi-ui-button-sound-018.wav");
			isBgLoaded = true;

		}
		DrawMenu(MainMenuBg);
		break;

	case GameState::PLAYING:
		DrawPlaying();
		break;
	case GameState::PAUSE:
		DrawPause(MainMenuBg);
		break;

	case GameState::GAMEOVER:
		DrawGameOver(MainMenuBg);
		break;
	}

}


void MainGame::Unload()

{

	Terrain::UnloadBackground();
	gameMap.UnloadMap();
	player.UnloadPlayer();
	allObjects.UnloadObjects();
	UnloadSound(ButtonSound);
	UnloadTexture(MainMenuBg);

}
