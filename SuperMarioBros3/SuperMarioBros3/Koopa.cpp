#include "Koopa.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "KoopaConst.h"

CKoopa::CKoopa()
{
	LoadAnimation();
	Init();
}

void CKoopa::Init()
{
	LoadAnimation();
	SetState(KOOPA_STATE_MOVE);
	isEnabled = true;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(KOOPA_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Koopa");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);


	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(KOOPA_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
}

void CKoopa::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(KOOPA_STATE_IDLE, animationManager->Get("ani-red-koopa-troopa-idle"));
	AddAnimation(KOOPA_STATE_MOVE, animationManager->Get("ani-red-koopa-troopa-move"));
	AddAnimation(KOOPA_STATE_SPIN, animationManager->Get("ani-red-koopa-troopa-spin"));
	AddAnimation(KOOPA_STATE_CROUCH, animationManager->Get("ani-red-koopa-troopa-crouch"));
}

void CKoopa::Update(DWORD dt, CCamera* cam)
{
}
