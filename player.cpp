#include "player.h"
#include "game.h"
#include "animation.h"
#include "tilemap.h"
#include <iostream>
// TODO <-Need to use variable in playerVelocity and playerPosition->
// <-Add death switch case->
// <-Add climb switch case (optional)>

Player::Player() : PlayerDirection(Direction::RIGHT), PlayerVelocity({ 2.2,2 }), PlayerPosition({16,272}),currentPlayerState(PlayerState::IDLE),
jumpForce(-5.4f), isJumping(false), gravity(0.3f), verticalJumpVelocity(0.0f), Groundlevel({ 16,272 }) {
}

Player::~Player(){}
Animation Anim;

     float playerWidth = 14.0f;
     float playerHeight = 15.0f;

     void Player::PlayerPositionUpdate(Vector2 PlayerPosition)
     {

         bool moving = false;
         float dx = 0.0f;
         float dy = 0.0f;


         for (auto it = SavePointQueue.begin(); it != SavePointQueue.end(); ++it) {
             if (CheckCollisionRecs({ this->PlayerPosition.x, this->PlayerPosition.y, playerWidth, playerHeight }, *it)) {
                 Groundlevel.x = it->x;
                 Groundlevel.y = it->y;
                 SavePointQueue.erase(it); // Remove the collided save point
                 break; // Important: break after erase to avoid iterator issues
             }
         }



   
    if (IsKeyDown(KEY_D) && this->currentPlayerState!=PlayerState::DEAD)
    {
        this->currentPlayerState = PlayerState::RUNNING;
        this->PlayerDirection = Direction::RIGHT;
        dx = this->PlayerVelocity.x;
        moving = true;
    }
    else if (IsKeyDown(KEY_A) && this->currentPlayerState != PlayerState::DEAD)
    {
        this->currentPlayerState = PlayerState::RUNNING;
        this->PlayerDirection = Direction::LEFT;
        dx = -this->PlayerVelocity.x;
        moving = true;
    }


  
    if (dx != 0.0f)
    {
        Rectangle futureH = {
            this->PlayerPosition.x + dx ,
            this->PlayerPosition.y,
            playerWidth,
            playerHeight
        };

        bool hitH = false;
        for (int i = 0; i < (int)GridCollisionCoord.size(); i++)
        {
            if (CheckCollisionRecs(futureH, GridCollisionCoord[i]))
            {
                hitH = true;

                break;
            }
        }

        if (!hitH)
        {
            this->PlayerPosition.x += dx;
        }
    }

   
    Rectangle feetRect = {
        this->PlayerPosition.x,
        this->PlayerPosition.y + playerHeight,  
        playerWidth,
        2.0f                                  
    };

    bool onGround = false;
    for (int i = 0; i < (int)GridCollisionCoord.size(); i++)
    {
        if (CheckCollisionRecs(feetRect, GridCollisionCoord[i]))
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

    
    if (IsKeyPressed(KEY_SPACE) && !isJumping && this->currentPlayerState != PlayerState::DEAD) // can be onGround (Need to look into it)
    {
        isJumping = true;
        verticalJumpVelocity = jumpForce;   // (negative if jumpForce < 0)
        this->currentPlayerState = PlayerState::JUMPING;
    }



    if (isJumping)
    {

        verticalJumpVelocity += gravity;
        dy = verticalJumpVelocity;
    }

    if (dy != 0.0f)
    {
        
        Rectangle futureV = {
            this->PlayerPosition.x,
            this->PlayerPosition.y + dy,
            playerWidth,
            playerHeight
        };

        bool hitV = false;
        for (int i = 0; i < (int)GridCollisionCoord.size(); i++)
        {
            Rectangle tileRect = GridCollisionCoord[i];
            if (CheckCollisionRecs(futureV, tileRect))
            {
                hitV = true;

                if (dy > 0.0f)
                {
                    
                    this->PlayerPosition.y = tileRect.y - playerHeight;
                    isJumping = false;
                    verticalJumpVelocity = 0.0f;
                    this->currentPlayerState = moving ? PlayerState::RUNNING
                        : PlayerState::IDLE;
                }
                else 
                {
                    this->PlayerPosition.y = tileRect.y + tileRect.height;
                    verticalJumpVelocity = 0.0f;
                    
                }
                break;
            }
        }

        if (!hitV)
        {
    
            this->PlayerPosition.y += dy;
        }
    }

 
    if (IsKeyPressed(KEY_Q) && this->currentPlayerState != PlayerState::DEAD)
    {
        this->currentPlayerState = PlayerState::ATTACK;
    }

    if (this->currentPlayerState == PlayerState::ATTACK && Anim.isFinished())
    {
        this->currentPlayerState = PlayerState::IDLE;
    }

    if (!moving
        && !isJumping
        && this->currentPlayerState != PlayerState::ATTACK && this->currentPlayerState != PlayerState::DEAD)
    {
        this->currentPlayerState = PlayerState::IDLE;
    }
    for (int i = 0;i < ObjectCollisionCoord.size();i++) {
        if (CheckCollisionRecs({ PlayerPosition.x, PlayerPosition.y, playerWidth+1, playerHeight+1 }, ObjectCollisionCoord[i])) {
            this->currentPlayerState = PlayerState::DEAD;
            break;
        }
    }

    if (this->currentPlayerState == PlayerState::DEAD && Anim.isFinished())
    {
            this->PlayerPosition.x = Groundlevel.x; 
            this->PlayerPosition.y = Groundlevel.y;
            this->PlayerDirection = Direction::RIGHT;
            this->currentPlayerState = PlayerState::IDLE;
    }

    if (this->PlayerPosition.y >= 1000) {
        this->PlayerPosition.x = Groundlevel.x; 
        this->PlayerPosition.y = Groundlevel.y;
        this->PlayerDirection = Direction::RIGHT;
        this->currentPlayerState = PlayerState::IDLE;
    }
}


static Texture PlayerSkin[5] = {0};

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

void Player::DrawPlayer(Animation& PlayerAnim)

{
	Anim = PlayerAnim;
	if (PlayerSkin[0].id != 0)

	{
		Vector2 playerpos = this->PlayerPosition;
        switch (this->currentPlayerState)
        {
            // the size of the player is something which is giving problem
        case IDLE:
            Rectangle playerIdle = PlayerAnim.animationFrame(4, AnimationType::REPEATING);
            playerIdle.width *= this->PlayerDirection;
            if (PlayerDirection == Direction::LEFT) {
                DrawTexturePro(PlayerSkin[0], playerIdle, { playerpos.x-8,playerpos.y - 8,24,24 }, { 0,0 }, 0.0f, WHITE);
            }
            else {
                DrawTexturePro(PlayerSkin[0], playerIdle, { playerpos.x,playerpos.y - 8,24,24 }, { 0,0 }, 0.0f, WHITE);
            }
           // DrawRectangleLinesEx({ playerpos.x, playerpos.y, playerWidth, playerHeight }, 1, RED);
			break;
		case RUNNING:
			Rectangle playerRun = PlayerAnim.animationFrame(6, AnimationType::REPEATING);
			playerRun.width *= this->PlayerDirection;
            if (PlayerDirection == Direction::LEFT) {
                DrawTexturePro(PlayerSkin[1], playerRun, { playerpos.x - 8,playerpos.y - 8,24,24 }, { 0,0 }, 0.0f, WHITE);
            }
            else {
                DrawTexturePro(PlayerSkin[1], playerRun, { playerpos.x,playerpos.y - 8,24,24 }, { 0,0 }, 0.0f, WHITE);
            }
           // DrawRectangleLinesEx({ playerpos.x, playerpos.y, playerWidth, playerHeight }, 1, RED);
			break;
		case JUMPING:
			Rectangle playerJump = PlayerAnim.animationFrame(4, AnimationType::ONESHOT);
			playerJump.width *= this->PlayerDirection;
            if (PlayerDirection == Direction::LEFT) {
                DrawTexturePro(PlayerSkin[2], playerJump, { playerpos.x - 8,playerpos.y - 8,24,24 }, { 0,0 }, 0.0f, WHITE);
            }
            else {
                DrawTexturePro(PlayerSkin[2], playerJump, { playerpos.x,playerpos.y - 8,24,24 }, { 0,0 }, 0.0f, WHITE);
            }
           // DrawRectangleLinesEx({ playerpos.x, playerpos.y, playerWidth, playerHeight }, 1, RED);
			break;
		case ATTACK:
			Rectangle playerAttack = PlayerAnim.animationFrame(8, AnimationType::ONESHOT);
			playerAttack.width *= this->PlayerDirection;
            if (PlayerDirection == Direction::LEFT) {
                DrawTexturePro(PlayerSkin[3], playerAttack, { playerpos.x - 8,playerpos.y - 8,24,24 }, { 0,0 }, 0.0f, WHITE);
            }
            else {
                DrawTexturePro(PlayerSkin[3], playerAttack, { playerpos.x,playerpos.y - 8,24,24 }, { 0,0 }, 0.0f, WHITE);
            }
           // DrawRectangleLinesEx({ playerpos.x, playerpos.y, playerWidth, playerHeight }, 1, RED);
			break;
		case DEAD:
            Rectangle playerDead = PlayerAnim.animationFrame(6, AnimationType::ONESHOT);
            if (PlayerDirection == Direction::LEFT) {
                DrawTexturePro(PlayerSkin[4], playerDead, { playerpos.x - 8,playerpos.y - 8,24,24 }, { 0,0 }, 0.0f, WHITE);
            }
            else {
                DrawTexturePro(PlayerSkin[4], playerDead, { playerpos.x,playerpos.y - 8,24,24 }, { 0,0 }, 0.0f, WHITE);
            }
         //   DrawRectangleLinesEx({ playerpos.x, playerpos.y, playerWidth, playerHeight }, 1, RED);
			break;
		default:
			break;
		}



	}
}

void Player::UnloadPlayer()

{
    for (int i = 0; i < 5; i++)
    {
        UnloadTexture(PlayerSkin[i]);
    }
}