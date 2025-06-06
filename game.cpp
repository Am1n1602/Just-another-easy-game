#include "game.h"
#include "terrain.h"
#include "player.h"
#include "animation.h"
#include "tilemap.h"
#include "objects.h"

// Initial the global variables


MainGame::MainGame() : currentState(GameState::MENU), isLoaded(false),
camera() {
}
MainGame::~MainGame() {}

//<- TODO: Add the player class and imply it ->//
// <-Update 1: Partially done player class (idle,run,attack,jump) ->

//<--TODO: Add camera class and Background scrolling>
// <-Update 2: both done->

// <--TODO: Make Camera more smoother-->
// Done, Added Camera Smoothness

int MainGame::DeathCount = 0;


Player player;
Animation playerAnim;
Animation object;
TileMap gameMap;
Objects allObjects;

Vector2 PlayerStartingPosition = player.PlayerPosition;
const float CAMERA_SMOOTHNESS = 0.05f;

void MainGame::InitialCamera()
{
	camera.offset.x = 0.0f; // One-third horizontally
	camera.offset.y = 0.0f; // Center vertically
	camera.target = { 0,0 };
	camera.rotation = 0.0f;
	camera.zoom = 2.2f; // 2.2f for final
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
	DrawText(start, SCREENWIDTH / 2 - startWidth / 2, 2 * SCREENHEIGHT / 3, 30, DARKGRAY);

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



void MainGame::DrawPlaying()

{

	BeginMode2D(camera);
	Terrain::DrawBackground(player);
	gameMap.DrawMap();
	player.DrawPlayer(playerAnim);
	allObjects.DrawObjects(object,TYPEOBJECT::COIN);
	DrawRectanglePro({ camera.target.x+10,camera.target.y+10,130,40 }, { 0,0 }, 0.0f, TransParentGray);
	DrawRectangleLinesEx({ camera.target.x + 10,camera.target.y + 10,130,40 }, 3.0f, WHITE);
	Font fort = GetFontDefault();
	Vector2 Temp = {camera.target.x + 20, camera.target.y + 20};
	DrawTextEx(fort,TextFormat("DEATH COUNT: %d", DeathCount),Temp,10,1,RAYWHITE);
	
	
	EndMode2D();
	

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

		isLoaded = true;
	}

	player.PlayerPositionUpdate(player.PlayerPosition);
	playerAnim.AnimationUpdate(deltaTime);
	object.AnimationUpdate(deltaTime);
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


void MainGame::Unload()

{

	Terrain::UnloadBackground();
	gameMap.UnloadMap();
	player.UnloadPlayer();

}