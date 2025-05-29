#include "player.h"
#include "game.h"

PlayerAnimation::PlayerAnimation() : firstFrame(0),lastFrame(3),currentFrame(0),
animationSpeed(0.1),durationLeft(0.1) {}

PlayerAnimation::~PlayerAnimation() {}

void PlayerAnimation::PlayerAnimationUpdate(float deltaTime)

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

Rectangle PlayerAnimation::animationFrame(int numberFrameperRow) const

{
	int x = (currentFrame % numberFrameperRow) * 48;
	int y = (currentFrame / numberFrameperRow) * 48;

	return Rectangle({ (float)x,(float)y,48.0,48.0 });
}

static Texture PlayerSkin = { 0 };

void Player::LoadPlayer()

{
	if (PlayerSkin.id == 0)

	{
		PlayerSkin = LoadTexture("assets/Biker_idle.png");
	}
}

void Player::DrawPlayer(const PlayerAnimation& player)

{
	if (PlayerSkin.id != 0)

	{
		Rectangle PlayerFrameRate = player.animationFrame(4);
		DrawTexturePro(PlayerSkin,PlayerFrameRate, { 100,100,48*2,48*2 }, { 0,0 }, 0.0f, WHITE);
	}
}