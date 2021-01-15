#pragma once
#include "GameObject.h"
class CBrick : public CGameObject
{
	int type;
	CObjectPool brickPool;
	int bounceState;
	float bounceDelta;
	int startBounceTime;

public:
	CBrick();
	void LoadAnimation();
	void Init();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha = 255);
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
	void SetType(int type);
	int GetType();
	void Debris();
	void Bounce();
	CObjectPool GetObjectPool();
};

