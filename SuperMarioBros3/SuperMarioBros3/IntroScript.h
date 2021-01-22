#pragma once
#include "GameObject.h"
class CIntroScript : public CGameObject
{
	int cutSceneStages;
public:
	CIntroScript();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam);
	void Render(CCamera* cam, int alpha = 255);
};

