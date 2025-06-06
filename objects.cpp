#include "objects.h"

Objects::Objects() {}
Objects::~Objects() {}

Player Objplayer;


static Texture2D Coin = { 0 };
static Texture2D HeartEffect = { 0 };

void Objects::LoadObjects()

{
	if (Coin.id == 0)

	{
		Coin = LoadTexture("assets/Card.png");
	}
	if (HeartEffect.id == 0)

	{
		HeartEffect = LoadTexture("assets/lost_hearts_anim_strip_5.png");
	}
}

void Objects::DrawObjects(Animation Object,TYPEOBJECT currentObjectType)
{
    Vector2 heart1 = {this->PositionX,this->PositionY }; // Moved outside the switch block

    switch (currentObjectType)
    {
    case COIN:
        Rectangle savePoint = Object.animationFrame(8, AnimationType::REPEATING, 24);
        for (int i = 0; i < SavePointQueue.size(); i++)
        {
            DrawTexturePro(Coin, savePoint, { SavePointQueue[i].x, SavePointQueue[i].y, 40, 40 }, { 0, 0 }, 0.0f, WHITE);
        }
        break;

    case HEART:
        Rectangle heartAnim = Object.animationFrame(4, AnimationType::ONESHOT, 16);
        DrawTexturePro(HeartEffect, heartAnim, { heart1.x, heart1.y, 16, 16 }, { 0, 0 }, 0.0f, WHITE);
        break;

    default:
        break;
    }
}


