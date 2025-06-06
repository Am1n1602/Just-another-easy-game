#pragma once

#include <raylib.h>
#include "animation.h"
#include "tilemap.h"
#include "player.h"

enum TYPEOBJECT {
	COIN,
	HEART,
};

class Objects
{
public:
	Objects();
	~Objects();
	void LoadObjects();
	void DrawObjects(Animation Object,TYPEOBJECT);
	float PositionX;
	float PositionY;

private:

};

