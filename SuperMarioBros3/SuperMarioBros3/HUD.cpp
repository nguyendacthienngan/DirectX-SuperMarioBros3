#include "HUD.h"
#include "SpriteManager.h"
#include <iostream>
#include <string>
using namespace std;
CHUD::CHUD(D3DXVECTOR2 hudPos)
{
	this->pos = hudPos;
	auto pMeterPos = hudPos;
	pMeterPos.x -= 60;
	pMeterPos.y -= 11;

	timer = new CGameTimer();
	auto timerPos = hudPos;
	timerPos.x += 160;
	timerPos.y += 15;
	timer->SetPosition(timerPos);

	scoreText = new CFont();
	auto scorePos = hudPos;
	scorePos.x -= 60;
	scorePos.y += 15;
	scoreText->SetCurrentText("0000000");
	scoreText->SetPosition(scorePos);

	coinText = new CFont();
	auto coinPos = hudPos;
	coinPos.x += 185;
	coinPos.y -= 11;
	coinText->SetCurrentText("00");
	coinText->SetPosition(coinPos);

	auto cardPos = hudPos;
	cardPos.x += 300;
	pMeter = new CPMeter(pMeterPos);
	card1 = new CCardGift(cardPos);
	cardPos.x += 24 * 3;
	card2 = new CCardGift(cardPos);
	cardPos.x += 24 * 3;

	card3 = new CCardGift(cardPos);
	cardPos.x += 24 * 3;

	score = 0;
	coin = 0;
	LoadSprite();
}

void CHUD::LoadSprite()
{
	auto spriteManager = CSpriteManager::GetInstance();
	hudSprite = spriteManager->Get("spr-hud-0");
}

void CHUD::Update()
{
	pMeter->Update();
	card1->Update();
	card2->Update();
	card3->Update();
	timer->Update();

	auto scoreString = to_string(score);
	while (scoreString.length() < 7)
		scoreString = "0" + scoreString;
	scoreText->SetCurrentText(scoreString);

	auto coinString = to_string(coin);
	while (coinString.length() < 2)
		coinString = "0" + coinString;
	coinText->SetCurrentText(coinString);
}

void CHUD::Render()
{
	hudSprite->Draw(pos, D3DXVECTOR2(1.0f, 1.0f), 0.0f);
	pMeter->Render();
	card1->Render();
	card2->Render();
	card3->Render();
	timer->Render();
	coinText->Render();
	scoreText->Render();
}

void CHUD::SetPosition(D3DXVECTOR2 pos)
{
	this->pos = pos;
}

D3DXVECTOR2 CHUD::GetPosition()
{
	return pos;
}

void CHUD::SetPMeter(CPMeter* pMeter)
{
	this->pMeter = pMeter;
}

CPMeter* CHUD::GetPMeter()
{
	return pMeter;
}

void CHUD::SetCard(int index, std::string state)
{
	if (index == 1)
		card1->SetState(state);
	else if (index == 2)
		card2->SetState(state);
	else
		card3->SetState(state);
}

CCardGift* CHUD::GetCard(int index)
{
	if (index == 1)
		return card1;
	else if (index == 2)
		return card2;
	else
		return card3;
}

CGameTimer* CHUD::GetTimer()
{
	return timer;
}

void CHUD::AddingScore(int score)
{
	this->score += score;
}

int CHUD::GetScore()
{
	return score;
}

void CHUD::AddingCoin(int coin)
{
	this->coin += coin;
}

int CHUD::GetCoin()
{
	return coin;
}
