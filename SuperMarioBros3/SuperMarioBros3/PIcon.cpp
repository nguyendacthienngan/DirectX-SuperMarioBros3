#include "PIcon.h"
#include "SpriteManager.h"
CPIcon::CPIcon()
{
	LoadSprite();
	isCharged = false;
}

void CPIcon::LoadSprite()
{
	auto spriteManager = CSpriteManager::GetInstance();
	blackPowerIconSprite = spriteManager->Get("spr-p-icon-black");
	whitePowerIconSprite = spriteManager->Get("spr-p-icon-white");
}

void CPIcon::Render()
{
	if (isCharged == false)
		currentSprite = blackPowerIconSprite;
	else 
		currentSprite = whitePowerIconSprite;
	currentSprite->Draw(pos, D3DXVECTOR2(1.0f, 1.0f), 0.0f);
}

void CPIcon::SetPosition(D3DXVECTOR2 pos)
{
	this->pos = pos;
}
