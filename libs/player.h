#pragma once

#include <raylib.h>
#include "animation.h"
#include "objects.h"

enum Direction
{
	LEFT = -1,
	RIGHT = 1,
};

enum PlayerState
{
	IDLE,
	RUNNING,
	JUMPING,
	ATTACK,
	DEAD,
};

class Player
{

private:
	const float jumpForce;
	const float gravity;
	float verticalJumpVelocity;
	Vector2 Groundlevel;
	bool isJumping;
	PlayerState currentPlayerState;
	Direction PlayerDirection;
	Vector2 PlayerVelocity;

public:
	Vector2 PlayerPosition;
	Player();
	~Player();
	void PlayerPositionUpdate(Vector2 PlayerPosition,bool &isReset,bool &isWin);
	static void LoadPlayer();
	void LoadPlayerSound();
	void DrawPlayer(Animation& PlayerAnim);
	static void UnloadPlayer();
};
