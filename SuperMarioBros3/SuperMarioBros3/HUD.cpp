#include "HUD.h"
#include "SpriteManager.h"

CHUD::CHUD(D3DXVECTOR2 hudPos)
{
	this->pos = hudPos;
	auto pMeterPos = hudPos;
	pMeterPos.x -= 60;
	pMeterPos.y -= 11;
	auto cardPos = hudPos;
	cardPos.x += 300;
	pMeter = new CPMeter(pMeterPos);
	card1 = new CCardGift(cardPos);
	cardPos.x += 24 * 3;
	card2 = new CCardGift(cardPos);
	cardPos.x += 24 * 3;

	card3 = new CCardGift(cardPos);
	cardPos.x += 24 * 3;

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
}

void CHUD::Render()
{
	hudSprite->Draw(pos, D3DXVECTOR2(1.0f, 1.0f), 0.0f);
	pMeter->Render();
	card1->Render();
	card2->Render();
	card3->Render();
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
