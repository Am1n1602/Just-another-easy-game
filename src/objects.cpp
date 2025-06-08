#include "objects.h"
#include <iostream>
Objects::Objects(): currentTypeObject(LAVA) {}
Objects::~Objects() {}

Player Objplayer;

std::vector <Rectangle> LavaPoint;
static Texture2D Coin = { 0 };
static Texture2D HeartEffect = { 0 };
static Texture2D LavaEffect = { 0 };


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
    if (LavaEffect.id == 0) {
        LavaEffect = LoadTexture("assets/lava.png");
    }
}


void Objects::DrawObjects(Animation Object, TYPEOBJECT currentObjectType)
{
    Vector2 heart1 = { this->PositionX,this->PositionY }; // Moved outside the switch block

    this->currentTypeObject = currentObjectType;
    switch (this->currentTypeObject)
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

    case LAVA:
        Rectangle lavaAnim = Object.animationFrame(4, AnimationType::REPEATING, 16);
        for (int i = 0; i < LavaPoint.size(); i++)
        {
            DrawTexturePro(LavaEffect, lavaAnim, LavaPoint[i], { 0,0 }, 0.0f, WHITE);
        }
        break;

    default:
        break;
    }

}

    void Objects::UnloadObjects()

    {
        UnloadTexture(Coin);
        UnloadTexture(HeartEffect);
        UnloadTexture(LavaEffect);
    }



