#include "player.h"
#include "game.h"
#include "animation.h"
#include "tilemap.h"
#include <iostream>


// VERY IMPORTANT DO NOT CHANGE PLAYERVELOCITY IN X **SPOILER: IT WILL BREAK A MAJOR OBSTACLE 

Player::Player() : PlayerDirection(Direction::RIGHT), PlayerVelocity({ 2.42,2 }), PlayerPosition({ 32,544 }), currentPlayerState(PlayerState::IDLE),
jumpForce(-6.0f), isJumping(false), gravity(0.3f), verticalJumpVelocity(0.0f), Groundlevel({ 32,544 }) {
}
Player::~Player() {}

Animation anim;
Objects Heart;
MainGame DeathCounter;
Sound SavePointSound;
Sound DeathSound;
Sound GameOverSound;

float playerWidth = 28.0f;
float playerHeight = 28.0f;

void Player::PlayerPositionUpdate(Vector2 PlayerPosition,bool &isReset,bool &isWin)
{
	bool moving = false;
	float dx = 0.0f;
	float dy = 0.0f;

	if (isReset)
	{
		SavePointQueue = SavePointQueueBackup;
		this->PlayerPosition.x = 32;
		this->PlayerPosition.y = 544;
		Groundlevel.x = 32;
		Groundlevel.y = 544;
		isWin = false;
		isReset = false;
	}

	if (CheckCollisionRecs({ this->PlayerPosition.x, this->PlayerPosition.y, playerWidth, playerHeight }, { 1248,576,playerWidth,playerHeight }))
	{
		isWin=true;
		PlaySound(GameOverSound);
	}

	for (auto it = SavePointQueue.begin(); it != SavePointQueue.end(); ++it)

	{
		if (CheckCollisionRecs({ this->PlayerPosition.x, this->PlayerPosition.y, playerWidth, playerHeight }, *it))

		{
			PlaySound(SavePointSound);
			Groundlevel.x = it->x;
			Groundlevel.y = it->y;
			SavePointQueue.erase(it); // Remove the collided save point
			break; // Important: break after erase to avoid iterator issues

		}

		
		
	}

	if (IsKeyDown(KEY_D) && this->currentPlayerState != PlayerState::DEAD )
	{
		this->currentPlayerState = PlayerState::RUNNING;
		this->PlayerDirection = Direction::RIGHT;
		dx = this->PlayerVelocity.x;
		moving = true;
	}

	else if (IsKeyDown(KEY_A) && this->currentPlayerState != PlayerState::DEAD )
	{

		this->currentPlayerState = PlayerState::RUNNING;
		this->PlayerDirection = Direction::LEFT;
		dx = -this->PlayerVelocity.x;
		moving = true;
	}

	if (dx != 0.0f)

	{
		Rectangle futureHorizontalPosition = { this->PlayerPosition.x + dx ,this->PlayerPosition.y,playerWidth,playerHeight };
		bool hitHorizontal = false;

		for (int i = 0; i < (int)GridCollisionCoord.size(); i++)
		{
			if (CheckCollisionRecs(futureHorizontalPosition, GridCollisionCoord[i]))
			{
				hitHorizontal = true;
				break;
			}
		}

		if (!hitHorizontal)
		{
			this->PlayerPosition.x += dx;
		}
	}

	Rectangle feetRectangle = { this->PlayerPosition.x,this->PlayerPosition.y + playerHeight,  playerWidth,2.0f };
	bool onGround = false;

	for (int i = 0; i < (int)GridCollisionCoord.size(); i++)
	{
		if (CheckCollisionRecs(feetRectangle, GridCollisionCoord[i]))
		{
			onGround = true;
			break;
		}
	}

	if (!onGround && !isJumping)
	{
		isJumping = true;
		verticalJumpVelocity = 0.0f;        // start falling straight down
		this->currentPlayerState = PlayerState::JUMPING;
	}

	if (IsKeyPressed(KEY_SPACE) && !isJumping && this->currentPlayerState != PlayerState::DEAD) 
	{
		isJumping = true;
		verticalJumpVelocity = jumpForce;   // (negative if jumpForce < 0)
		this->currentPlayerState = PlayerState::JUMPING;
	}

	if (isJumping)
	{
		verticalJumpVelocity += gravity*0.0166720*45; //it was supposed to be deltatime but then it was becoming unpredictable so i hard code it.
		dy = verticalJumpVelocity;
	}

	if (dy != 0.0f)
	{
		Rectangle futureVerticalPosition = { this->PlayerPosition.x,this->PlayerPosition.y + dy,playerWidth, playerHeight };
		bool hitVertical = false;
		for (int i = 0; i < (int)GridCollisionCoord.size(); i++)
		{
			Rectangle tileRect = GridCollisionCoord[i];

			if (CheckCollisionRecs(futureVerticalPosition, tileRect))
			{
				hitVertical = true;
				if (dy > 0.0f)
				{
					this->PlayerPosition.y = tileRect.y - playerHeight;
					isJumping = false;
					verticalJumpVelocity = 0.0f;
					this->currentPlayerState = moving ? PlayerState::RUNNING : PlayerState::IDLE;
				}

				else
				{
					this->PlayerPosition.y = tileRect.y + tileRect.height;
					verticalJumpVelocity = 0.0f;
				}
				break;
			}
		}
		if (!hitVertical)
		{
			this->PlayerPosition.y += dy;
		}
	}

	for (int i = 0;i < ObjectCollisionCoord.size();i++)
	{
		if (CheckCollisionRecs({ PlayerPosition.x, PlayerPosition.y, playerWidth + 1, playerHeight +1 }, ObjectCollisionCoord[i]))
		{
			PlaySound(DeathSound);
			this->currentPlayerState = PlayerState::DEAD;	
			break;
		}
	}

	if (IsKeyPressed(KEY_Q) && this->currentPlayerState != PlayerState::DEAD)
	{
		this->currentPlayerState = PlayerState::ATTACK;
	}

	if (this->currentPlayerState == PlayerState::ATTACK && anim.isFinished())
	{
		this->currentPlayerState = PlayerState::IDLE;
	}

	if (this->currentPlayerState == PlayerState::DEAD && anim.isFinished())
	{
		this->PlayerPosition.x = Groundlevel.x;
		this->PlayerPosition.y = Groundlevel.y;
		this->PlayerDirection = Direction::RIGHT;
		this->currentPlayerState = PlayerState::IDLE;
		DeathCounter.DeathCount += 1;
	}

	if (!moving && !isJumping && this->currentPlayerState != PlayerState::ATTACK && this->currentPlayerState != PlayerState::DEAD)
	{
		this->currentPlayerState = PlayerState::IDLE;
	}

	if (this->PlayerPosition.y >= 1000)
	{
		PlaySound(DeathSound);
		this->PlayerPosition.x = Groundlevel.x;
		this->PlayerPosition.y = Groundlevel.y;
		this->PlayerDirection = Direction::RIGHT;
		this->currentPlayerState = PlayerState::IDLE;
		DeathCounter.DeathCount += 1;
	}
}

static Texture PlayerSkin[5] = { 0 };

void Player::LoadPlayer()

{
	if (PlayerSkin[0].id == 0)
	{
		PlayerSkin[0] = LoadTexture("assets/Biker_idle.png");
	}

	if (PlayerSkin[1].id == 0)
	{
		PlayerSkin[1] = LoadTexture("assets/Biker_run.png");
	}

	if (PlayerSkin[2].id == 0)
	{
		PlayerSkin[2] = LoadTexture("assets/Biker_jump.png");
	}

	if (PlayerSkin[3].id == 0)
	{
		PlayerSkin[3] = LoadTexture("assets/Biker_attack2.png");
	}

	if (PlayerSkin[4].id == 0) {
		PlayerSkin[4] = LoadTexture("assets/Biker_death.png");
	}
}

void Player::LoadPlayerSound()
{
	DeathSound = LoadSound("assets/350988__cabled_mess__lose_c_04.wav");
	SavePointSound = LoadSound("assets/535449__tetoszka__ukulele-simple-sound.wav");
	GameOverSound = LoadSound("assets/173859__jivatma07__j1game_over_mono.wav");
}

void Player::DrawPlayer(Animation& PlayerAnim)
{
	anim = PlayerAnim;
	if (PlayerSkin[0].id != 0)
	{
		Vector2 playerpos = this->PlayerPosition;
		Rectangle DrawPlayerPositionLeft = { (playerpos.x - playerWidth / 2),playerpos.y - playerHeight / 2,1.5f * playerWidth,playerHeight * 1.5f };
		Rectangle DrawPlayerPositionRight = { playerpos.x,playerpos.y - playerHeight / 2,1.5f * playerWidth,playerHeight * 1.5f };
		switch (this->currentPlayerState)
		{
		case IDLE:
		{
			Rectangle playerIdle = PlayerAnim.animationFrame(4, AnimationType::REPEATING, 48);
			playerIdle.width *= this->PlayerDirection;
			if (PlayerDirection == Direction::LEFT)
			{
				DrawTexturePro(PlayerSkin[0], playerIdle, DrawPlayerPositionLeft, { 0,0 }, 0.0f, WHITE);
			}
			else
			{
				DrawTexturePro(PlayerSkin[0], playerIdle, DrawPlayerPositionRight, { 0,0 }, 0.0f, WHITE);
			}
		/*	DrawRectangleLinesEx({ playerpos.x, playerpos.y, playerWidth + 1, playerHeight + 1 }, 1, RED);*/
			break;
		}

		case RUNNING:
		{
			Rectangle playerRun = PlayerAnim.animationFrame(6, AnimationType::REPEATING, 48);
			playerRun.width *= this->PlayerDirection;

			if (PlayerDirection == Direction::LEFT)
			{
				DrawTexturePro(PlayerSkin[1], playerRun, DrawPlayerPositionLeft, { 0,0 }, 0.0f, WHITE);
			}
			else
			{
				DrawTexturePro(PlayerSkin[1], playerRun, DrawPlayerPositionRight, { 0,0 }, 0.0f, WHITE);
			}
		/*	DrawRectangleLinesEx({ playerpos.x, playerpos.y,  playerWidth + 1, playerHeight + 1 }, 1, RED);*/
			break;
		}

		case JUMPING:
		{
			Rectangle playerJump = PlayerAnim.animationFrame(4, AnimationType::ONESHOT, 48);
			playerJump.width *= this->PlayerDirection;

			if (PlayerDirection == Direction::LEFT)
			{
				DrawTexturePro(PlayerSkin[2], playerJump, DrawPlayerPositionLeft, { 0,0 }, 0.0f, WHITE);
			}

			else
			{
				DrawTexturePro(PlayerSkin[2], playerJump, DrawPlayerPositionRight, { 0,0 }, 0.0f, WHITE);
			}
			/*DrawRectangleLinesEx({ playerpos.x, playerpos.y,  playerWidth + 1, playerHeight + 1 }, 1, RED);*/
			break;
		}

		case ATTACK:
		{
			Rectangle playerAttack = PlayerAnim.animationFrame(8, AnimationType::ONESHOT, 48);
			playerAttack.width *= this->PlayerDirection;

			if (PlayerDirection == Direction::LEFT)
			{
				DrawTexturePro(PlayerSkin[3], playerAttack, DrawPlayerPositionLeft, { 0,0 }, 0.0f, WHITE);
			}

			else
			{
				DrawTexturePro(PlayerSkin[3], playerAttack, DrawPlayerPositionRight, { 0,0 }, 0.0f, WHITE);
			}
		/*	DrawRectangleLinesEx({ playerpos.x, playerpos.y,  playerWidth + 1, playerHeight + 1 }, 1, RED);*/
			break;
		}

		case DEAD:
		{
			Rectangle playerDead = PlayerAnim.animationFrame(6, AnimationType::ONESHOT, 48);
			Heart.PositionX = playerpos.x;
			Heart.PositionY = playerpos.y - 10;
			Heart.DrawObjects(PlayerAnim, TYPEOBJECT::HEART);
			if (PlayerDirection == Direction::LEFT)
			{
				DrawTexturePro(PlayerSkin[4], playerDead, DrawPlayerPositionLeft, { 0,0 }, 0.0f, WHITE);
			}

			else
			{
				DrawTexturePro(PlayerSkin[4], playerDead, DrawPlayerPositionRight, { 0,0 }, 0.0f, WHITE);
			}
			//DrawRectangleLinesEx({ playerpos.x, playerpos.y,  playerWidth + 1, playerHeight + 1 }, 1, RED);
			break;
		}
		default:
		{
			break;
		}
	}
	}
}

void Player::UnloadPlayer()
{
	for (int i = 0; i < 5; i++)
	{
		UnloadTexture(PlayerSkin[i]);
	}
	UnloadSound(DeathSound);
	UnloadSound(SavePointSound);
	UnloadSound(GameOverSound);
}