#pragma once
#include "Misc.h"
#include "ObjectPool.h"
class CVenusFireBall :
    public CMisc
{
private:
	CObjectPool* pool;
public:
	CVenusFireBall();
	~CVenusFireBall();

	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void LoadAnimation() override;
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
	void LinkToPool(CObjectPool* pool);
};

