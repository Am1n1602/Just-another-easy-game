#pragma once

#include <raylib.h>
#include "animation.h"
#include "tilemap.h"
#include "player.h"
	extern std::vector <Rectangle> LavaPoint;

enum TYPEOBJECT {
	COIN,
	HEART,
	LAVA,
};

class Objects
{
public:
	Objects();
	~Objects();
	void LoadObjects();
	TYPEOBJECT currentTypeObject;
	void DrawObjects(Animation Object,TYPEOBJECT);
	float PositionX;
	float PositionY;
	void UnloadObjects();

private:

};

