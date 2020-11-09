#pragma once
#include "Koopa.h"
class CRedKoopa: public CKoopa
{
public:
	void Update(DWORD dt, CCamera* cam) override;
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;

};

