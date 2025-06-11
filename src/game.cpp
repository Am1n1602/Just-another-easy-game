#include "game.h"
#include "terrain.h"
#include "player.h"
#include "animation.h"
#include "tilemap.h"
#include "objects.h"
#include "leaderboard.h"
#include <thread>
#include <mutex>
#include <iomanip>


static std::vector<LeaderboardEntry> cachedTimeEntries;
static std::vector<LeaderboardEntry> cachedDeathEntries;
static std::mutex leaderboardMutex;

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

bool isLddrawn = false;
int MainGame::DeathCount = 0;
float MainGame::TotalTime = 0;
int ldTime = 0;
bool SwitchMenu = false;
bool PauseToResume = false;
bool PauseToExit = false;
static bool flushed = false;
bool MenuToExit = false;
bool isReset = false;
bool isWin = false;
bool timeORdeath = false;
bool toEntry = false;
static std::string name = "";
static int lettercount = 0;
static double lastTime = 0;
const double fetchInt = 60;
int FinalMin = 0;
int FinalSec = 0;
int FinalMs = 0;
static bool isFetching = false;
Sound ButtonSound;
bool isLeaderboard = false;
GameState Laststate = GameState::LEADERBOARD;



Player player;
Animation playerAnim;
Animation object;
TileMap gameMap;
Objects allObjects;

Vector2 PlayerStartingPosition = player.PlayerPosition;

const float CAMERA_SMOOTHNESS = 0.04f;

void MainGame::FetchInBackground() {
	try {
		std::vector<LeaderboardEntry> timeData = FetchLeaderboard("time");
		std::vector<LeaderboardEntry> deathData = FetchLeaderboard("deaths");

		{
			std::lock_guard<std::mutex> lock(leaderboardMutex);
			cachedTimeEntries = std::move(timeData);
			cachedDeathEntries = std::move(deathData);
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Fetch error: " << e.what() << std::endl;
	}

	isFetching = false;
}

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
	
	double currentTime = GetTime();

	if (((currentTime - lastTime > fetchInt) || lastTime == 0) && !isFetching) {
		lastTime = currentTime;

		// Mark fetching BEFORE starting thread to avoid race conditions
		isFetching = true;

		std::thread fetchThread(&MainGame::FetchInBackground, this);
		fetchThread.detach();
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

	if (TotalTime <= 10) {

		DrawText("PRESS D for Right Movement ", PlayerStartingPosition.x + 40, PlayerStartingPosition.y - 70, 10, YELLOW);
		DrawText("PRESS A for Left Movement", PlayerStartingPosition.x + 40, PlayerStartingPosition.y - 50, 10, YELLOW);
		DrawText("PRESS SPACE for Jump", PlayerStartingPosition.x + 40, PlayerStartingPosition.y - 30, 10, YELLOW);

	}

	if (TotalTime <= 15 && TotalTime > 10)
	{
		DrawText("HAVE FUN !", PlayerStartingPosition.x + 40, PlayerStartingPosition.y - 50, 20, YELLOW);
	}

	DrawRectangleRounded({ camera.target.x + 10,camera.target.y + 10,130,40 }, 0.2, 1, BlueBlueTrans);

	DrawRectangleRoundedLinesEx({ camera.target.x + 10,camera.target.y + 10,130,40 }, 0.3, 1, 2, LIGHTGRAY);

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

float alpha = 1.0f;
float fadespeed = -0.005f;

void MainGame::DrawGameOver(Texture2D MainMenuBg)

{

	Color FadingBlackGround = { 0,0,0,(int)(alpha * 255) };
	DrawTexturePro(MainMenuBg, { 0,0,(float)MainMenuBg.width,(float)MainMenuBg.height }, { 0,0,SCREENWIDTH,SCREENHEIGHT }, { 0,0 }, 0.0f, WHITE);
	DrawRectangleRounded({ 0.3*SCREENWIDTH,0.44*SCREENHEIGHT,SCREENWIDTH/3,SCREENHEIGHT/5 }, 0.2, 1, VibrantOrange);
	DrawRectangleRoundedLinesEx({ 0.3 * SCREENWIDTH,0.44 * SCREENHEIGHT,SCREENWIDTH / 3,SCREENHEIGHT / 5 }, 0.3, 1, 2, WHITE);
	DrawTextPro(GetFontDefault(), "!!! Congratulations !!! ", {0.35 * SCREENWIDTH,0.46 * SCREENHEIGHT}, {0,0}, 0.0f, 34, 2, BLACK);
	DrawTextPro(GetFontDefault(), TextFormat("You took %d mins %d secs %d ms", FinalMin, FinalSec, FinalMs), { 0.317 * SCREENWIDTH, 0.52 * SCREENHEIGHT }, { 0,0 }, 0.0f, 27, 2, BLACK);
	DrawTextPro(GetFontDefault(), TextFormat("You died only %d times", DeathCount), { 0.323 * SCREENWIDTH, 0.57 * SCREENHEIGHT }, { 0,0 }, 0.0f, 27, 2, BLACK);
	DrawRectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, FadingBlackGround);

	if (toEntry)
	{
		const int screenWidth = 1280;
		const int screenHeight = 720;

		const int boxWidth = 500;
		const int boxHeight = 60;
		const int fontSize = 30;

		const int inputX = (screenWidth - boxWidth) / 2;
		const int inputY = (screenHeight - boxHeight) / 2;

		// Draw label above box
		DrawRectangle(SCREENWIDTH/4,0.3*SCREENHEIGHT,SCREENWIDTH / 2,SCREENHEIGHT / 3 ,BLACK);
		DrawText("Enter your name:", inputX, inputY - 40, 32, SKYBLUE);

		// Input box background and border
		DrawRectangleRounded({ (float)inputX, (float)inputY, (float)boxWidth, (float)boxHeight }, 0.2f, 4, ORANGE);
		DrawRectangleRoundedLinesEx( { (float)inputX, (float)inputY, (float)boxWidth, (float)boxHeight }, 0.2f, 4, 2, SKYBLUE);

		// Draw name text
		DrawText(name.c_str(), inputX + 15, inputY + 15, fontSize, DARKGRAY);

		// Blinking cursor
		if ((GetTime() - floor(GetTime())) < 0.5) {
			int textWidth = MeasureText(name.c_str(), fontSize);
			DrawText("|", inputX + 15 + textWidth + 2, inputY + 15, fontSize, MAROON);
		}

		// Max character warning
		if (lettercount >= 15) {
			const char* warn = "Maximum 15 characters allowed!";
			int warnWidth = MeasureText(warn, 20);
			DrawText(warn, screenWidth / 2 - warnWidth / 2, inputY + boxHeight + 15, 20, RED);
		}
	}

}

void MainGame::DrawLeaderboard()
{
	DrawRectangleRounded({ 0.1f * SCREENWIDTH, 0.1f * SCREENHEIGHT, 0.8f * SCREENWIDTH, 0.8f * SCREENHEIGHT }, 0.12f, 1, TransParentGray);
	DrawTextPro(GetFontDefault(), "LEADERBOARD", { 0.35f * SCREENWIDTH, 0.14f * SCREENHEIGHT }, { 0,0 }, 0.0f, 50, 3, BLACK);

	// Table headers
	int startX = static_cast<int>(0.15f * SCREENWIDTH);
	int startY = static_cast<int>(0.22f * SCREENHEIGHT);
	int rowHeight = 40;
	int colRank = startX+50;
	int colName = startX+200;
	int colDeaths = startX + 500;
	int colTime = startX + 700;

	DrawText("RANK", colRank-50, startY, 30, DARKGRAY);
	DrawText("NAME", colName, startY, 30, DARKGRAY);
	DrawText("DEATHS", colDeaths, startY, 30, DARKGRAY);
	DrawText("TIME", colTime, startY, 30, DARKGRAY);

	// GetTime() returns seconds since start


	if (IsKeyPressed(KEY_T)) {
		timeORdeath = true;
	}
	if (IsKeyPressed(KEY_D)) {
		timeORdeath = false;
	}

	std::vector<LeaderboardEntry> timeentries, deathentries;
	{
		std::lock_guard<std::mutex> lock(leaderboardMutex);
		timeentries = cachedTimeEntries;
		deathentries = cachedDeathEntries;
	}

	if (timeORdeath) {
		// Show time leaderboard
		for (int i = 0; i < std::min(10, (int)timeentries.size()); ++i) {
			const auto& entry = timeentries[i];
			int y = startY + rowHeight * (i + 1);
			DrawText(TextFormat("%d", i+1), colRank, y, 28, BLACK);
			DrawText(entry.name.c_str(), colName, y, 28, BLACK);
			DrawText(TextFormat("%d", entry.deaths), colDeaths, y, 28, BLACK);
			int minutes = entry.time / 60000;
			int seconds = (entry.time % 60000) / 1000;
			int milliseconds = entry.time % 1000;
			DrawText(TextFormat("%02d:%02d:%03d ms", minutes, seconds, milliseconds), colTime, y, 28, BLACK);
		}
	}
	else {
		// Show death leaderboard
		for (int i = 0; i < std::min(10, (int)deathentries.size()); ++i) {
			const auto& entry = deathentries[i];
			int y = startY + rowHeight * (i + 1);
			DrawText(TextFormat("%d", i + 1), colRank, y, 28, BLACK);
			DrawText(entry.name.c_str(), colName, y, 28, BLACK);
			DrawText(TextFormat("%d", entry.deaths), colDeaths, y, 28, BLACK);
			int minutes = entry.time / 60000;
			int seconds = (entry.time % 60000) / 1000;
			int milliseconds = entry.time % 1000;
			DrawText(TextFormat("%02d:%02d:%03d ms", minutes, seconds, milliseconds), colTime, y, 28, BLACK);
		}
	}

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
	if (IsKeyPressed(KEY_L) && !isLeaderboard)
	{
		currentState = GameState::LEADERBOARD;
		isLeaderboard = true;
		Laststate = GameState::PLAYING;
	}

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

	if (IsKeyPressed(KEY_L) && !isLeaderboard)
	{
		currentState = GameState::LEADERBOARD;
		isLeaderboard = true;
		Laststate = GameState::MENU;
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

	if (IsKeyPressed(KEY_L) && !isLeaderboard)
	{
		currentState = GameState::LEADERBOARD;
		isLeaderboard = true;
		Laststate = GameState::PAUSE;
	}
}

void MainGame::UpdateGameOver()

{
	alpha += fadespeed;

	if (alpha <= 0)
	{
		alpha = 0.0f;
		fadespeed = 0;
	}

	
	int death = DeathCount;
	float endtime = ((FinalMin * 60000) + (FinalSec * 1000) + FinalMs);

	if (!toEntry && GetKeyPressed()) {
    toEntry = true;
    flushed = false;  // Set flush flag
}

// Step 2: Handle name input
if (toEntry) {
    // Step 2.1: Flush any leftover keypresses for 1 frame
    if (!flushed) {
        while (GetKeyPressed()) {}     // Flush regular keys
        while (GetCharPressed()) {}    // Flush char inputs
        flushed = true;
    }
    else {
        // Step 2.2: Handle name input
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (lettercount < 15)) {
                name += static_cast<char>(key);
                lettercount++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && lettercount > 0) {
            name.pop_back();
            lettercount--;
        }

        // Draw input UI
        DrawText("Enter your name:", 100, 100, 30, BLACK);
        DrawText(name.c_str(), 100, 140, 28, DARKGRAY);

        if (IsKeyPressed(KEY_ENTER)) {
            std::string playername(name);
			SubmitScore(playername, death, endtime);
            std::cout << playername << " " << death << " " << endtime << "\n";

            // Reset
			name.clear();
			lettercount = 0;
			currentState = GameState::MENU;
			isReset = true;
			alpha = 1.0f;
			fadespeed = -0.005f;
			toEntry = false;
        }
    }
}
}

void MainGame::UpdateLeaderboard()
{
	if (IsKeyPressed(KEY_L) || IsKeyPressed(KEY_ESCAPE))
	{
		if (Laststate == GameState::MENU)
		{
			currentState = GameState::MENU;
		}
		else if (Laststate == GameState::PLAYING)
		{
			currentState = GameState::PLAYING;
		}
		else if (Laststate == GameState::PAUSE)
		{
			currentState = GameState::PAUSE;
		}
		isLeaderboard = false;
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
	case GameState::LEADERBOARD:
		UpdateLeaderboard();

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
	case GameState::LEADERBOARD:
			DrawLeaderboard();
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
