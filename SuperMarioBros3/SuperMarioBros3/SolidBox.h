#pragma once
#include "GameObject.h"
// CollisionBox là của framework
// SolidBox là 1 instance từ CollsionBox khi ta load từ map vô
class CSolidBox : public CGameObject
{
public:
	CSolidBox();
	void Update(DWORD dt, CCamera* cam) { };
	void Init();
	void KeyState() { }
	void OnKeyDown(int KeyCode) { }
	void OnKeyUp(int KeyCode) { }
};

