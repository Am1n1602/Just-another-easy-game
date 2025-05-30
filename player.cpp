#include "player.h"
#include "game.h"
#include "animation.h"


// TODO <-Need to use variable in playerVelocity and playerPosition->

Player::Player() : PlayerDirection(Direction::RIGHT), PlayerVelocity({ 5,5 }), PlayerPosition({30,600}),currentPlayerState(PlayerState::IDLE) {}

Player::~Player(){}

// TODO <-To fix jumping logic and animation->

void Player::PlayerPositionUpdate(Vector2 PlayerPosition)  

{

	bool moving = false;

   if (IsKeyDown(KEY_D))  
   {  
	   this->currentPlayerState = PlayerState::RUNNING;
       this->PlayerDirection = Direction::RIGHT; 
	   this->PlayerPosition.x += this->PlayerVelocity.x;
	   moving = true;
   }  
   if (IsKeyDown(KEY_A))
   {
	   this->currentPlayerState = PlayerState::RUNNING;
	   this->PlayerDirection = Direction::LEFT;
	   this->PlayerPosition.x -= this->PlayerVelocity.x;
	   moving = true;
   }


   if (IsKeyPressed(KEY_SPACE))
   {
	   this->currentPlayerState = PlayerState::JUMPING;
	   
   }
   if (!moving && this->currentPlayerState != PlayerState::JUMPING)
   {
	   this->currentPlayerState = PlayerState::IDLE;
   }
}




static Texture PlayerSkin[3] = {0};

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
}

void Player::DrawPlayer(Animation& PlayerAnim)

{
	int offset = -24;
	if (PlayerSkin[0].id != 0)

	{
		Vector2 playerpos = this->PlayerPosition;
		switch (this->currentPlayerState)
		{
		case IDLE:
			Rectangle playerIdle = PlayerAnim.animationFrame(4);
			playerIdle.width *= this->PlayerDirection;
			DrawTexturePro(PlayerSkin[0], playerIdle, {playerpos.x,playerpos.y,48 * 2,48 * 2}, {0,0}, 0.0f, WHITE);
			break;
		case RUNNING:
			Rectangle playerRun = PlayerAnim.animationFrame(6);
			playerRun.width *= this->PlayerDirection;
			DrawTexturePro(PlayerSkin[1], playerRun, { playerpos.x,playerpos.y,48 * 2,48 * 2}, {0,0}, 0.0f, WHITE);
			break;
		case JUMPING:
			Rectangle playerJump = PlayerAnim.animationFrame(4);
			DrawTexturePro(PlayerSkin[2], playerJump, {playerpos.x,playerpos.y+offset,48 * 2,48 * 2 }, { 0,0 }, 0.0f, WHITE);
			break;
		case ATTACK:
			break;
		case DEAD:
			break;
		default:
			break;
		}

	}
}