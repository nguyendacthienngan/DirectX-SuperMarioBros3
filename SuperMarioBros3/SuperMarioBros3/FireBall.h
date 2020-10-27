#pragma once
#include "Misc.h"
class CFireBall : public CMisc
{
private:
	CGameObject* fireMario;
public:
	CFireBall();
	~CFireBall();
	void PhysicsUpdate(std::vector<LPGameObject>* coObjects) override;

	void Update(DWORD dt, CCamera* cam) override;
	void LoadAnimation() override;
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
};

