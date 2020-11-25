#pragma once
#include "Koopa.h"
class CRedKoopa: public CKoopa
{
public:
	CRedKoopa();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;
	void LoadAnimation() override;
};

