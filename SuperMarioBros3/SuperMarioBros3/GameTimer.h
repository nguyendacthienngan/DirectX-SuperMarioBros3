#pragma once
#include "Font.h"
class CGameTimer
{
	D3DXVECTOR2 pos;
	CFont* timerFont;
	DWORD currentTime;
	int timerState;
public:
	CGameTimer();
	void Update();
	void Render();
	void SetPosition(D3DXVECTOR2 pos);
	D3DXVECTOR2 GetPosition();
	void StartTimer();
	void ResetTimer();
	CFont* GetTimerText();
};

