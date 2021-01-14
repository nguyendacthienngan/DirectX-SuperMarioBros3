#include "MovingPlatform.h"
#include "AnimationManager.h"
#include "MovingPlatformConst.h"
CMovingPlatform::CMovingPlatform()
{
	LoadAnimation();
	SetState("PLATFORM");
	isEnabled = true;
	isFall = false;

	tag = GameObjectTags::MovingPlatform;

	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(MOVING_PLATFORM_BBOX);
	box->SetGameObjectAttach(this);
	this->collisionBoxs->push_back(box);
	this->physiscBody->SetDynamic(true);
	this->physiscBody->SetGravity(0);
	fallForce = MOVING_PLATFORM_GRAVITY*2;
}

void CMovingPlatform::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("PLATFORM", animations->Get("ani-platform"));
}

void CMovingPlatform::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	if (isFall == false)
	{
		this->physiscBody->SetVelocity(D3DXVECTOR2(MOVING_PLATFORM_SPEED, 0));
	}
	else
	{
		this->physiscBody->SetVelocity(D3DXVECTOR2(0, fallForce));
	}
}

bool CMovingPlatform::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (MarioTag(tag))
		return true;
	return false;
}

void CMovingPlatform::SetFall(bool isFall)
{
	this->isFall = isFall;
}

void CMovingPlatform::SetFallForce(float fallForce)
{
	this->fallForce = fallForce;
}

float CMovingPlatform::GetFallForce()
{
	return fallForce;
}
