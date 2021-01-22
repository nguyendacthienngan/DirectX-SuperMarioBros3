#pragma once
#include "GameObject.h"
#include "MarioScript.h"
class CIntroScript : public CGameObject
{
	CGameObject* curtain;
	CMarioScript* marioScript;
	int cutSceneStages;
public:
	CIntroScript();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam);
	void Render(CCamera* cam, int alpha = 255);
};

