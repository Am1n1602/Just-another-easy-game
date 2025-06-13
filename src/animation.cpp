#include "animation.h"


Animation::Animation() : firstFrame(0), currentFrame(0), lastFrame(3),
animationSpeed(0.1), durationLeft(0.1), currentAnimationType(AnimationType::REPEATING) {
}

Animation::~Animation() {}



int GlobalFrame = 0;

Rectangle Animation::animationFrame(int numberFrameperRow, AnimationType AnimationType, int Width)

{

	// Need to replace 48 with player width

	GlobalFrame = numberFrameperRow - 1;
	this->currentAnimationType = AnimationType;
	int x = (currentFrame % numberFrameperRow) * Width;
	int y = (currentFrame / numberFrameperRow) * Width;

	return Rectangle({ static_cast<float>(x),static_cast<float>(y),static_cast<float>(Width),static_cast<float>(Width) });
}

void Animation::AnimationUpdate(float deltaTime)

{
	lastFrame = GlobalFrame;
	durationLeft -= deltaTime;
	if (durationLeft <= 0.0)

	{
		durationLeft = animationSpeed;
		currentFrame++;


		if (currentFrame > lastFrame)

		{
			switch (this->currentAnimationType)
			{
			case AnimationType::REPEATING:
				currentFrame = firstFrame;
				break;

			case AnimationType::ONESHOT:
				currentFrame = lastFrame;

			default:
				break;
			}

		}
	}
}

bool Animation::isFinished() const

{
	return currentAnimationType == AnimationType::ONESHOT && currentFrame == lastFrame && durationLeft == animationSpeed;
}
