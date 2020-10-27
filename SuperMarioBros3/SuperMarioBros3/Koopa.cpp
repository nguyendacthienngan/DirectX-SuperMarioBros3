#include "Koopa.h"
#include "AnimationManager.h"
#include "EnemyConst.h"
CKoopa::CKoopa()
{
	LoadAnimation();
	Init();

}

void CKoopa::Init()
{
	this->SetTag(GameObjectTags::Enemy);

	LoadAnimation();
	SetState(KOOPA_STATE_IDLE);
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
	AddAnimation(KOOPA_STATE_IDLE, animationManager->Get("ani-red-koopa-troopa-move"));
	AddAnimation(KOOPA_STATE_IDLE, animationManager->Get("ani-red-koopa-troopa-spin"));
	AddAnimation(KOOPA_STATE_IDLE, animationManager->Get("ani-red-koopa-troopa-crouch"));
}
