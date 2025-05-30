#pragma once

#include <raylib.h>

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

class PlayerAnimation  
{  
   int firstFrame;  
   int lastFrame;  
   int currentFrame;  
   float animationSpeed;  
   float durationLeft;  
public:  
	
	

   PlayerAnimation();  
   ~PlayerAnimation();  
   void PlayerAnimationUpdate(float deltaTime);  
   Rectangle animationFrame(int numberFrameperRow) const; // Marked as const to allow usage with const objects  
};   




class Player:PlayerAnimation

{
private:
	


public:

   PlayerState currentPlayerState;
	Vector2 PlayerVelocity;
	Vector2 PlayerPosition;
	Direction PlayerDirection;

	Player();
	~Player();

	void PlayerPositionUpdate(Vector2 PlayerPosition);

	static void LoadPlayer();
	void DrawPlayer(const PlayerAnimation& playerAnim);
};
