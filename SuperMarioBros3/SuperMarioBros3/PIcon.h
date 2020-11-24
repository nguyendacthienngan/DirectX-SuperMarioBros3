#pragma once
#include "Sprite.h"
class CPIcon
{
private:
	D3DXVECTOR2 pos;
	LPSprite whitePowerIconSprite;
	LPSprite blackPowerIconSprite;
	LPSprite currentSprite;
	bool isCharged;
public:
	CPIcon();
	void LoadSprite();
	void Render();
	void SetPosition(D3DXVECTOR2 pos);
};

