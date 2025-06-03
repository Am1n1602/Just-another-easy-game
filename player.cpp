#include "player.h"
#include "game.h"
#include "animation.h"
#include "tilemap.h"
#include <iostream>
// TODO <-Need to use variable in playerVelocity and playerPosition->
// <-Add death switch case->
// <-Add climb switch case (optional)>

Player::Player() : PlayerDirection(Direction::RIGHT), PlayerVelocity({ 5,5 }), PlayerPosition({128,588}),currentPlayerState(PlayerState::IDLE),
jumpForce(-10.0f),isJumping(false),gravity(0.5f),verticalJumpVelocity(0.0f),Groundlevel(608){}

Player::~Player(){}
Animation Anim;


void Player::PlayerPositionUpdate(Vector2 PlayerPosition)
{
    const float playerWidth = 32.0f;
    const float playerHeight = 96.0f;

    bool moving = false;
    float dx = 0.0f;
    float dy = 0.0f;

   
    if (IsKeyDown(KEY_D))
    {
        this->currentPlayerState = PlayerState::RUNNING;
        this->PlayerDirection = Direction::RIGHT;
        dx = this->PlayerVelocity.x;
        moving = true;
    }
    else if (IsKeyDown(KEY_A))
    {
        this->currentPlayerState = PlayerState::RUNNING;
        this->PlayerDirection = Direction::LEFT;
        dx = -this->PlayerVelocity.x;
        moving = true;
    }

  
    if (dx != 0.0f)
    {
        Rectangle futureH = {
            this->PlayerPosition.x + dx,
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

    
    if (IsKeyPressed(KEY_SPACE) && !isJumping) // can be onGround (Need to look into it)
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

 
    if (IsKeyPressed(KEY_Q))
    {
        this->currentPlayerState = PlayerState::ATTACK;
    }

    if (this->currentPlayerState == PlayerState::ATTACK && Anim.isFinished())
    {
        this->currentPlayerState = PlayerState::IDLE;
    }

    if (!moving
        && !isJumping
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
            // the size of the player is something which is giving problem
		case IDLE:
			Rectangle playerIdle = PlayerAnim.animationFrame(4,AnimationType::REPEATING);
			playerIdle.width *= this->PlayerDirection;
			DrawTexturePro(PlayerSkin[0], playerIdle, {playerpos.x,playerpos.y,64 ,96 }, {0,0}, 0.0f, WHITE);
			break;
		case RUNNING:
			Rectangle playerRun = PlayerAnim.animationFrame(6, AnimationType::REPEATING);
			playerRun.width *= this->PlayerDirection;
			DrawTexturePro(PlayerSkin[1], playerRun, { playerpos.x,playerpos.y,64 ,96 }, {0,0}, 0.0f, WHITE);
			break;
		case JUMPING:
			Rectangle playerJump = PlayerAnim.animationFrame(4, AnimationType::ONESHOT);
			playerJump.width *= this->PlayerDirection;
			DrawTexturePro(PlayerSkin[2], playerJump, {playerpos.x,playerpos.y,64 ,96  }, { 0,0 }, 0.0f, WHITE);
			break;
		case ATTACK:
			Rectangle playerAttack = PlayerAnim.animationFrame(8, AnimationType::ONESHOT);
			playerAttack.width *= this->PlayerDirection;
			DrawTexturePro(PlayerSkin[3],playerAttack, { playerpos.x,playerpos.y,64 ,96  }, { 0,0 }, 0.0f, WHITE);
			break;
		case DEAD:
			break;
		default:
			break;
		}



	}
}