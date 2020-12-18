#include "BrickEffect.h"
#include "AnimationManager.h"
#include "Game.h"
#include "BrickFXConst.h"
CBrickEffect::CBrickEffect()
{
	LoadAnimation();
	SetState("BRICK");
	SetScale(D3DXVECTOR2(0.70f, 0.70f));
	physiscBody->SetGravity(DEBRIS_GRAVITY);
	isEnabled = false;
}

void CBrickEffect::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("BRICK", animations->Get("ani-brick-debris"));
}

void CBrickEffect::LinkToPool(CObjectPool* pool)
{
	this->pool = pool;
}

void CBrickEffect::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto deltaTime = dt * CGame::GetTimeScale();
	auto vel = physiscBody->GetVelocity();

	auto pos = GetPosition();
	SetPosition(pos + vel * deltaTime);

	vel.y += physiscBody->GetGravity() * deltaTime;
	vel.y = (vel.y < DEBRIS_MIN_FALL) ? vel.y : DEBRIS_MIN_FALL;
	physiscBody->SetVelocity(vel);
}
