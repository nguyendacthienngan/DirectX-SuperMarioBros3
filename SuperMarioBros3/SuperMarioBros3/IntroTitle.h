#pragma once
#include "GameObject.h"
#include "Sprite.h"
class CIntroTitle : public CGameObject
{
	CSprite* titleSprites[4];
	bool isShowVersion;
	int titleAppearingState;
	D3DXVECTOR2 startPos;
public:
	CIntroTitle();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam);
	void Render(CCamera* cam, int alpha = 255);
};

