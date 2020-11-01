#pragma once
#include "Holdable.h"
#include "Enemy.h"
class CKoopaShell : public CEnemy, public CHoldable
{
protected:
	bool canRun, stopHold;
	bool isRun;
	bool headShot;
public:
	CKoopaShell();
	void Init();
	void LoadAnimation();

	void Update(DWORD dt, CCamera* cam) override;
	void Render(CCamera* cam) override;

	bool IsRunning();
	void SetRun();
	void SetStopRun();
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	void OnDie();

	virtual void SetHoldablePosition(D3DXVECTOR2 pos);
	virtual void Release();
	virtual D3DXVECTOR2 GetHoldableCollisionBox();
	virtual bool IsHolding();
};

