#pragma once
#include "Font.h"
class CGameTimer
{
	D3DXVECTOR2 pos;
	CFont* timerFont;
public:
	CGameTimer();
	void Update();
	void Render();
};

