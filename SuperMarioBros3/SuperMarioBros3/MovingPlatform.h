#pragma once
#include "GameObject.h"
class CMovingPlatform : public CGameObject
{
	bool isFall;
	float fallForce;
public:
	CMovingPlatform();
	void LoadAnimation() override;
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag);
	void SetFall(bool isFall);
	void SetFallForce(float fallForce);
	float GetFallForce();
};

