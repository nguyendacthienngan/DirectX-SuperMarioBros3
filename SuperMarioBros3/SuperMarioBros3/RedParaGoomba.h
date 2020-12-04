#pragma once
#include "RedGoomba.h"
class CRedParaGoomba: public CEnemy
{
	CRedGoomba* redGoomba;
public:
	CRedParaGoomba();
	void Init() override;
	void LoadAnimation() override;
	//void Render(CCamera* cam, int alpha);
	//virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	//virtual void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;
	//void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	//void ChangeToGoomba();
	//CRedGoomba* GetGoomba();
	//void SetGoomba(CRedGoomba* koopa);
	//bool IsOnGround();
	//void OnDamaged(CGameObject* otherGO) override;
};

