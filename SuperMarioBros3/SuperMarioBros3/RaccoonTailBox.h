#pragma once
#include "GameObject.h"
#include "RaccoonMario.h"
class CRaccoonTailBox: public CGameObject
{
public:
	CRaccoonTailBox();
	void Init() override;
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;
};

