#pragma once
#include "GameObject.h"
#include "MarioAuto.h"
#include "LuigiAuto.h"
#include "Goomba.h"
#include "MushroomEffect.h"
#include "KoopaShell.h"
#include "LeafEffect.h"

class CMarioScript : public CGameObject
{
private:
	CMarioAuto* marioController;
	CLuigiAuto* luigi;

	CGoomba* goomba;
	CMushroomEffect* mushroomFX;
	CLeafEffect* leafFX;
	CKoopaShell* koopaShell;

	int stage;
public:
	CMarioScript();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam);
	void Render(CCamera* cam, int alpha = 255);
	void SetStartAction();
};

