#pragma once
#include "GameObject.h"
// CollisionBox là của framework
// SolidBox là 1 instance từ CollsionBox khi ta load từ map vô
class CSolidBox : public CGameObject
{
public:
	CSolidBox();
	void Init();
	void KeyState(BYTE* states) { }
	void OnKeyDown(int KeyCode) { }
	void OnKeyUp(int KeyCode) { }
};

