#pragma once
#include "Enemy.h"
#include "GoombaConst.h"
class CGoomba : public CEnemy
{
private:
	GoombaState currentPhysicsState;
public:
	CGoomba();
	void Init() override;
	void LoadAnimation() override;
	void Update(DWORD dt, CCamera* cam) override;
	void Render(CCamera* cam) override;
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	virtual void OnDie();
};

