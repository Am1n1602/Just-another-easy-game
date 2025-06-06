#pragma once

#include <raylib.h>


enum class AnimationType

{
	REPEATING,
	ONESHOT,

};

class Animation

{

private:

	int firstFrame;
	int currentFrame;
	int lastFrame;
	float animationSpeed;
	float durationLeft;

public:

	bool isFinished() const;
	AnimationType currentAnimationType;
	Animation();
	~Animation();
	void AnimationUpdate(float deltaTime);
	Rectangle animationFrame(int numberFrameperRow, AnimationType current,int width);

};