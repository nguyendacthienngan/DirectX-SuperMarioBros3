#include "CVenus.h"
#include "AnimationManager.h"
#include "VenusConst.h"
CVenus::CVenus()
{
	LoadAnimation();
	Init();
}

void CVenus::Init()
{
	isEnabled = true;
	SetState(VENUS_STATE_STAND);

	/*enemyTag = EnemyTag::Piranha;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(VENUS_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Piranha");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));*/

}

void CVenus::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(VENUS_STATE_STAND, animationManager->Clone("ani-red-venus-fire-trap-stand"));
}

void CVenus::Update(DWORD dt, CCamera* cam)
{
}
