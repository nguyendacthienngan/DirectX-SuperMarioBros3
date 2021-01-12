#pragma once
#include "GameObject.h"
class CMovingPlatform : public CGameObject
{

public:
	CMovingPlatform();
	void LoadAnimation() override;
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag);
};

