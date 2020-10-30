#pragma once
#include "Enemy.h"
class CKoopaShell : public CEnemy
{
public:
	CKoopaShell();
	void Init();
	void LoadAnimation();
	void Update(DWORD dt, CCamera* cam) override;
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;

};

