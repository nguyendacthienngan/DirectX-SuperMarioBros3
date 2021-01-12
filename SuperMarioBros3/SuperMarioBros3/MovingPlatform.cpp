#include "MovingPlatform.h"
#include "AnimationManager.h"
#include "MovingPlatformConst.h"
CMovingPlatform::CMovingPlatform()
{
	LoadAnimation();
	SetState("PLATFORM");
	isEnabled = true;
	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(MOVING_PLATFORM_BBOX);
	box->SetGameObjectAttach(this);
	this->collisionBoxs->push_back(box);
	this->physiscBody->SetDynamic(true);
}

void CMovingPlatform::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("PLATFORM", animations->Get("ani-platform"));
}

void CMovingPlatform::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	this->physiscBody->SetVelocity(D3DXVECTOR2(MOVING_PLATFORM_SPEED, 0));
}

bool CMovingPlatform::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (MarioTag(tag))
		return true;
	return false;
}
