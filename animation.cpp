#include "animation.h"


Animation::Animation() : firstFrame(0), lastFrame(3), currentFrame(0),
animationSpeed(0.1), durationLeft(0.1) {
}

Animation::~Animation() {}


void Animation::AnimationUpdate(float deltaTime)

{
	durationLeft -= deltaTime;
	if (durationLeft <= 0.0)

	{
		durationLeft = animationSpeed;
		currentFrame++;

		if (currentFrame > lastFrame)

		{
			currentFrame = firstFrame;
		}
	}
}

Rectangle Animation::animationFrame(int numberFrameperRow) const

{
	int x = (currentFrame % numberFrameperRow) * 48;
	int y = (currentFrame / numberFrameperRow) * 48;

	return Rectangle({ (float)x,(float)y,48.0,48.0 });
}
