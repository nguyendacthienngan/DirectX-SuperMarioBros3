#pragma once
#include "Font.h"
class CGameTimer
{
	D3DXVECTOR2 pos;
	CFont* timerFont;
	int currentTime;
	int timerState;
public:
	CGameTimer();
	void Update();
	void Render();
	void SetPosition(D3DXVECTOR2 pos);
	D3DXVECTOR2 GetPosition();
	void StartTimer();
	void ResetTimer(); // Reset to 300
	void ResetToZero(); // Reset to 0
	CFont* GetTimerText();
	void TimerToFont();
	int GetTimerState();
};

