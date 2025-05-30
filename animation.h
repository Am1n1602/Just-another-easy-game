#pragma once

#include <raylib.h>

class Animation
{
	int firstFrame;
	int lastFrame;
	int currentFrame;
	float animationSpeed;
	float durationLeft;
public:



	Animation();
	~Animation();
	void AnimationUpdate(float deltaTime);
	Rectangle animationFrame(int numberFrameperRow) const; // Marked as const to allow usage with const objects  
};