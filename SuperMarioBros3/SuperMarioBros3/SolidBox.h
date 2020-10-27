#pragma once
#include "GameObject.h"
// CollisionBox là của framework
// SolidBox là 1 instance từ CollsionBox khi ta load từ map vô
class CSolidBox : public CGameObject
{
public:
	CSolidBox();
	void Init();
	~CSolidBox();
};

