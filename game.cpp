#include "game.h"
#include "terrain.h"
#include "player.h"
#include "animation.h"

// Initial the global variables

MainGame::MainGame() : currentState(GameState::MENU),isLoaded(false),
camera(){}
MainGame::~MainGame(){}

//<- TODO: Add the player class and imply it ->//
// <-Update 1: Partially done player class (idle,run,attack,jump) ->

//<--TODO: Add camera class and Background scrolling>

Player player;
Animation PlayerAnim;

Vector2 PlayerStartingPosition = player.PlayerPosition;


void MainGame::InitialCamera()

{
	this->camera.offset = PlayerStartingPosition;
	this->camera.target = player.PlayerPosition;
	this->camera.rotation = 0.0f;
	this->camera.zoom = 1.0f;

}

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


void MainGame::UpdateCamera(float deltaTime)

{
	Vector2 velocity = player.PlayerVelocity;

	camera.target = player.PlayerPosition;

}
void MainGame::DrawPlaying()

{
	InitialCamera();
	BeginMode2D(camera);
	Terrain::DrawBackground(player);
	player.DrawPlayer(PlayerAnim);
	EndMode2D();

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
	UpdateCamera(deltaTime);
	


	// if (IsKeyPressed(KEY_ESCAPE))
	//{
	//	currentState = GameState::MENU;
	//} Uncomment in the final version 

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


