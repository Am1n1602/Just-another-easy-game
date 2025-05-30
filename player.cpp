#include "player.h"
#include "game.h"
#include "animation.h"


// TODO <-Need to use variable in playerVelocity and playerPosition->

Player::Player() : PlayerDirection(Direction::RIGHT), PlayerVelocity({ 5,5 }), PlayerPosition({30,600}),currentPlayerState(PlayerState::IDLE),
jumpForce(-10.0f),isJumping(false),gravity(0.5f),verticalJumpVelocity(0.0f),Groundlevel(600){}

Player::~Player(){}
Animation Anim;

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


   if (IsKeyPressed(KEY_SPACE) && !isJumping)

   {
	   isJumping = true;
	   verticalJumpVelocity = jumpForce;
	   this->currentPlayerState = PlayerState::JUMPING;
	   
	   
   }

   if (isJumping)

   {
	   verticalJumpVelocity += gravity;
	   this->PlayerPosition.y += verticalJumpVelocity;

	   // <-TODO: Making this groundLevel game variable ->

	   if (this->PlayerPosition.y >= Groundlevel)

	   {
		   this->PlayerPosition.y >= Groundlevel;
		   isJumping = false;
		   verticalJumpVelocity = 0;
		   currentPlayerState = moving ? PlayerState::RUNNING : PlayerState::IDLE;
	   }
   }

   if (IsKeyPressed(KEY_Q))

   {
	   this->currentPlayerState = PlayerState::ATTACK;
   }

   if (currentPlayerState == PlayerState::ATTACK && Anim.isFinished())

   {
	   currentPlayerState = PlayerState::IDLE;
   }

   if (!moving && this->currentPlayerState != PlayerState::JUMPING
	   && this->currentPlayerState != PlayerState::ATTACK)

   {
	   this->currentPlayerState = PlayerState::IDLE;
   }
}


static Texture PlayerSkin[4] = {0};

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
}

void Player::DrawPlayer(Animation& PlayerAnim)

{
	Anim = PlayerAnim;
	if (PlayerSkin[0].id != 0)

	{
		Vector2 playerpos = this->PlayerPosition;
		switch (this->currentPlayerState)
		{

		case IDLE:
			Rectangle playerIdle = PlayerAnim.animationFrame(4,AnimationType::REPEATING);
			playerIdle.width *= this->PlayerDirection;
			DrawTexturePro(PlayerSkin[0], playerIdle, {playerpos.x,playerpos.y,48 * 2,48 * 2}, {0,0}, 0.0f, WHITE);
			break;
		case RUNNING:
			Rectangle playerRun = PlayerAnim.animationFrame(6, AnimationType::REPEATING);
			playerRun.width *= this->PlayerDirection;
			DrawTexturePro(PlayerSkin[1], playerRun, { playerpos.x,playerpos.y,48 * 2,48 * 2}, {0,0}, 0.0f, WHITE);
			break;
		case JUMPING:
			Rectangle playerJump = PlayerAnim.animationFrame(4, AnimationType::ONESHOT);
			playerJump.width *= this->PlayerDirection;
			DrawTexturePro(PlayerSkin[2], playerJump, {playerpos.x,playerpos.y,48 * 2,48 * 2 }, { 0,0 }, 0.0f, WHITE);
			break;
		case ATTACK:
			Rectangle playerAttack = PlayerAnim.animationFrame(8, AnimationType::ONESHOT);
			playerAttack.width *= this->PlayerDirection;
			DrawTexturePro(PlayerSkin[3],playerAttack, { playerpos.x,playerpos.y,48 * 2,48 * 2 }, { 0,0 }, 0.0f, WHITE);
			break;
		case DEAD:
			break;
		default:
			break;
		}

	}
}