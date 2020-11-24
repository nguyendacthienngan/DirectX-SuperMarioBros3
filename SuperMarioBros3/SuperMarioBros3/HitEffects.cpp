#include "HitEffects.h"
#include "AnimationManager.h"
#include "EffectConst.h"
#include "Ultis.h"

CHitEffects::CHitEffects()
{
	LoadAnimation();
	SetState(ENEMY_DAMAGED_EFFECT);
	startHitTime = 0;
}

void CHitEffects::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(ENEMY_DAMAGED_EFFECT, animationManager->Get("ani-enemy-damaged"), false);
	AddAnimation(FIRE_BALL_DAMAGED_EFFECT, animationManager->Get("ani-enemy-damaged"), false);
	AddAnimation("NONE", animationManager->Get("ani-fire-ball"), true);
}

void CHitEffects::EndAnimation()
{
}

void CHitEffects::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	if (GetTickCount64() - startHitTime > TIME_HEAD_SHOT_ANIMATION && startHitTime != 0)
		isEnabled = false;
}

void CHitEffects::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}

void CHitEffects::SetStartHitTime(DWORD t)
{
	startHitTime = t;
}
