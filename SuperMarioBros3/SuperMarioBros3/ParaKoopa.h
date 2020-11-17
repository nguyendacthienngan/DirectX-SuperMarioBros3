#pragma once
#include "Enemy.h"
#include "GreenKoopa.h"
class CParaKoopa : public CEnemy
{
	CGreenKoopa* koopa;
public:
	CParaKoopa();
	void Init() override;
	void LoadAnimation() override;
	void Render(CCamera* cam, int alpha);
	virtual void Update(DWORD dt, CCamera* cam) override;
	virtual void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	void ChangeToKoopa();
	CGreenKoopa* GetKoopa();
	void SetKoopa(CGreenKoopa* koopa);
};

