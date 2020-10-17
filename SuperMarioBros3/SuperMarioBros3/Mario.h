#pragma once

#ifndef MARIO_H
#define MARIO_H

#include "GameObject.h"
class CMario : public CGameObject
{
public:
	CMario();
	void Init();
	void LoadAnimation();
	void Update(DWORD dt, CCamera* cam);

	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	~CMario();
};
#endif
