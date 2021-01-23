#pragma once
#include "Sprite.h"
#include "GameObject.h"
class CCurtain: public CGameObject
{
private:
	CSprite* curtainSprite;
public: 
	CCurtain();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha = 255) override;
};

