#pragma once
#include "GameObject.h"
class CMarioMap: public CGameObject
{
private:

public:
	CMarioMap();
	void LoadAnimation();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha = 255) override;

};

