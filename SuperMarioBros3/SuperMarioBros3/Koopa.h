#pragma once
#include "Enemy.h"
#include "KoopaShell.h"
#include "KoopaConst.h"

class CKoopaShell;

class CKoopa: public CEnemy
{
protected:
	CKoopaShell* koopaShell;
	KoopaType koopaType;
public:
	CKoopa();
	void Init();
	virtual void LoadAnimation();
	void Render(CCamera* cam, int alpha = 255) override;
	virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;

	virtual void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	void ChangeToShell();
	void OnDie() override;
	void OnDamaged(CGameObject* otherGO) override;
	
	void ChangeBackToKoopa();

	void SetKoopaShell(CKoopaShell* koopaShell);
	CKoopaShell* GetKoopaShell();
	KoopaType GetKoopaType();
};

