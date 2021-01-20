#include "Boomerang.h"
#include "AnimationManager.h"
#include "BoomerangBrotherConst.h"

CBoomerang::CBoomerang()
{
	LoadAnimation();
	SetState(BOOMERANG_STATE_SPIN);
	isEnabled = true;
}

void CBoomerang::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(BOOMERANG_STATE_SPIN, animationManager->Clone("ani-boomerang-spin"));
}

void CBoomerang::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}

void CBoomerang::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
}

void CBoomerang::LinkToPool(CObjectPool* pool)
{
	this->pool = pool;
}
