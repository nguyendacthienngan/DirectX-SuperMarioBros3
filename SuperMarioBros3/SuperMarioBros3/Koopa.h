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
	void LoadAnimation();
	virtual void Update(DWORD dt, CCamera* cam) override;
	void Render(CCamera* cam, int alpha = 255) override;

	virtual void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	void ChangeToShell();
	void OnDie() override;
	void SetBoundary(float boundLeft, float boundRight);
	float GetBoundaryLeft();
	float GetBoundaryRight();
	
	void SetKoopaShell(CKoopaShell* koopaShell);
	CKoopaShell* GetKoopaShell();
};

