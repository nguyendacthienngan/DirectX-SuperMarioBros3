#include "Goomba.h"
#include "AnimationManager.h"
#include "Ultis.h"

CGoomba::CGoomba()
{
	CGoomba::Init();

}

void CGoomba::Init()
{
	LoadAnimation();
	SetState(GOOMBA_STATE_IDLE);
	isEnabled = true;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(GOOMBA_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Goomba");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);


	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(GOOMBA_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
}

void CGoomba::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(GOOMBA_STATE_IDLE, animationManager->Get("ani-goomba-idle"));
	AddAnimation(GOOMBA_STATE_WALK, animationManager->Get("ani-goomba-walk"));
	AddAnimation(GOOMBA_STATE_DIE, animationManager->Get("ani-goomba-die"));
}

void CGoomba::Update(DWORD dt, CCamera* cam)
{
	//DebugOut(L"Goomba Position Y: %f \n", transform.position.y);
}
