#pragma once
#include "Enemy.h"
#include "KoopaShell.h"

class CKoopaShell;

class CKoopa: public CEnemy
{
protected:
	CKoopaShell* koopaShell;
	float boundaryLeft, boundaryRight;
public:
	CKoopa();
	void Init();
	virtual void LoadAnimation();
	void Render(CCamera* cam, int alpha = 255) override;
	virtual void Update(DWORD dt, CCamera* cam) override;

	virtual void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	void ChangeToShell();
	void OnDie() override;
	void SetBoundary(float boundLeft, float boundRight);
	float GetBoundaryLeft();
	float GetBoundaryRight();
	
	void ChangeBackToKoopa();

	void SetKoopaShell(CKoopaShell* koopaShell);
	CKoopaShell* GetKoopaShell();
};

