#pragma once

#include <raylib.h>

enum Direction

{
	LEFT = -1,
	RIGHT = 1,
};

class PlayerAnimation  
{  
public:  
   int firstFrame;  
   int lastFrame;  
   int currentFrame;  
   float animationSpeed;  
   float durationLeft;  

   PlayerAnimation();  
   ~PlayerAnimation();  

   void PlayerAnimationUpdate(float deltaTime);  
   Rectangle animationFrame(int numberFrameperRow) const; // Marked as const to allow usage with const objects  
};   




class Player:PlayerAnimation

{
private:



public:

	static void LoadPlayer();
	static void DrawPlayer(const PlayerAnimation& player);
};
