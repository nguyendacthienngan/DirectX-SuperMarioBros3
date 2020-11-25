#include "Piranha.h"
#include "AnimationManager.h"
#include "PiranhaConst.h"
#include "Ultis.h"

CPiranha::CPiranha()
{
	CPiranha::LoadAnimation();
	CPiranha::Init();
}

void CPiranha::Init()
{
	isEnabled = true;
	SetState(PIRANHA_STATE_ATTACK);

	enemyTag = EnemyTag::Piranha;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(PIRANHA_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Piranha");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
}

void CPiranha::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(PIRANHA_STATE_ATTACK, animationManager->Clone("ani-green-piranha-plant-attack"));
}

void CPiranha::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	CPlant::Update(dt, cam, uiCam);
}