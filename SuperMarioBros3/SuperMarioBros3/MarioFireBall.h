#pragma once
#include "Misc.h"
#include "ObjectPool.h"
class CMarioFireBall : public CMisc
{
private:
	CObjectPool* pool;
public:
	CMarioFireBall();
	~CMarioFireBall();
	void PhysicsUpdate(std::vector<LPGameObject>* coObjects) override;

	void Update(DWORD dt, CCamera* cam) override;
	void LoadAnimation() override;
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);

	void LinkToPool(CObjectPool* pool);
};

