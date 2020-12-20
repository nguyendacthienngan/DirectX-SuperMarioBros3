#include "GameTimer.h"
#include <iostream> 
#include <string> 
#include "Ultis.h"
#include "Game.h"
using namespace std;
CGameTimer::CGameTimer()
{
	timerState = 0;
	currentTime = 300 * 1000;
	timerFont = new CFont();
	timerFont->SetSpace(D3DXVECTOR2(8*3, 0));
	timerFont->SetCurrentText("000");
}

void CGameTimer::Update()
{
	auto dt = CGame::GetInstance()->GetDeltaTime();

	
	if (timerState == 1)
	{
		currentTime -= dt;
		if (currentTime >= 1000)
			TimerToFont();
	}
	if (timerState == 2)
	{
		if (currentTime >= 1000)
		{
			currentTime -= dt * 200;
			TimerToFont();
		}
	}
	if (currentTime < 1000 && timerState != 0)
	{
		ResetTimer();
		return;
	}
}

void CGameTimer::Render()
{
	timerFont->Render();
}

void CGameTimer::SetPosition(D3DXVECTOR2 pos)
{
	this->pos = pos;
	timerFont->SetPosition(pos);
}

D3DXVECTOR2 CGameTimer::GetPosition()
{
	return pos;
}

void CGameTimer::StartTimer()
{
	timerState = 1;
	currentTime = 300 * 1000;
}

void CGameTimer::ResetTimer()
{
	timerState = 0;
	currentTime = 0;
	timerFont->SetCurrentText("000");
}

void CGameTimer::ResetToZero()
{
	timerState = 2;
}

CFont* CGameTimer::GetTimerText()
{
	return timerFont;
}

void CGameTimer::TimerToFont()
{
	if (currentTime < 1000 || currentTime < 0)
		return;
	string timerText = to_string((int)((float)currentTime * 0.001f)); // miliseconds to seconds
	while (timerText.length() < 3 && timerText.length() > 0)
		timerText = "0" + timerText;
	timerFont->SetCurrentText(timerText);
}

int CGameTimer::GetTimerState()
{
	return timerState;
}
