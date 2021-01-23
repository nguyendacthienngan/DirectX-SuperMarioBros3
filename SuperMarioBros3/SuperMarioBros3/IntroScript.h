#pragma once
#include "GameObject.h"
#include "MarioScript.h"
#include "IntroTitle.h"
class CIntroScript : public CGameObject
{
	CGameObject* curtain;
	CMarioScript* marioScript;
	CIntroTitle* introTitle;
	int cutSceneStages;
public:
	CIntroScript();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam);
	void Render(CCamera* cam, int alpha = 255);
};

