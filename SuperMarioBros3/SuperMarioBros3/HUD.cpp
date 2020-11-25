#include "HUD.h"
#include "SpriteManager.h"

CHUD::CHUD(D3DXVECTOR2 hudPos)
{
	this->pos = hudPos;
	auto pMeterPos = hudPos;
	pMeterPos.x -= 60;
	pMeterPos.y -= 11;
	pMeter = new CPMeter(pMeterPos);
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
}

void CHUD::Render()
{
	hudSprite->Draw(pos, D3DXVECTOR2(1.0f, 1.0f), 0.0f);
	pMeter->Render();
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
