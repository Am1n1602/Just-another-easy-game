#pragma once

#include <raylib.h>
#include "animation.h"


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
	float Groundlevel;

public:

	bool isJumping;
	PlayerState currentPlayerState;
	Vector2 PlayerVelocity;
	Vector2 PlayerPosition;
	Direction PlayerDirection;
	Player();
	~Player();
	void PlayerPositionUpdate(Vector2 PlayerPosition);
	static void LoadPlayer();
	void DrawPlayer(Animation& PlayerAnim);

};
