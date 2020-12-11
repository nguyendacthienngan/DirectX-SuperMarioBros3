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
	if (currentTime < 1000)
	{
		ResetTimer();
		return;
	}
	if (timerState == 1)
	{
		auto dt = CGame::GetInstance()->GetDeltaTime();
		currentTime -= dt;
		string timerText = to_string((int)((float)currentTime * 0.001f)); // miliseconds to seconds
		while (timerText.length() < 3 && timerText.length() > 0)
			timerText = "0" + timerText;
		timerFont->SetCurrentText(timerText);
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
}

void CGameTimer::ResetTimer()
{
	timerState = 0;
	currentTime = 300 * 1000;
}

CFont* CGameTimer::GetTimerText()
{
	return timerFont;
}
