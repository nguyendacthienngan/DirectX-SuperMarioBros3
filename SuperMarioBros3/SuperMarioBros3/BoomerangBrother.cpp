#include "BoomerangBrother.h"
#include "AnimationManager.h"
#include "BoomerangBrotherConst.h"
#include "Boomerang.h"

CBoomerangBrother::CBoomerangBrother()
{
	enemyTag = EnemyTag::BoomerangBrother;
	LoadAnimation();
	SetState(BOOMERANG_STATE_MOVE);
	isEnabled = true;
	for (int i = 0; i < 2; i++)
	{
		CBoomerang* boomerang = new CBoomerang();
		boomerang->LinkToPool(&boomerangs);
		boomerangs.Add(boomerang);
	}
	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(BOOMERANG_BROTHER__BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Boomerang");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(BOOMERANG_BROTHER_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
}

void CBoomerangBrother::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(BOOMERANG_STATE_MOVE, animationManager->Clone("ani-boomerang-brother-move"));
	AddAnimation(BOOMERANG_STATE_ATTACK, animationManager->Clone("ani-boomerang-brother-attack"));
}

void CBoomerangBrother::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}

void CBoomerangBrother::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
}

CObjectPool CBoomerangBrother::GetObjectPool()
{
	return boomerangs;
}
