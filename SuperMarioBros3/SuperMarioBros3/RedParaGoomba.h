#pragma once
#include "RedGoomba.h"
class CRedParaGoomba: public CEnemy
{
	CRedGoomba* redGoomba;
	int jumpState; // 0: Find Mario, 1: Rest, 2,3,4: Low Jump, 5: High Jump, 6: Stop
	int jumpStep; // 0,1,2: Low Jump ?
	DWORD timeStartFixing;
public:
	CRedParaGoomba();
	void Init() override;
	void LoadAnimation() override;
	void Render(CCamera* cam, int alpha);
	virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	/*virtual void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	void ChangeToGoomba();
	CRedGoomba* GetGoomba();
	void SetGoomba(CRedGoomba* koopa);*/
	bool IsOnGround();
	//void OnDamaged(CGameObject* otherGO) override;
};

