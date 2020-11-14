#include "HitEffects.h"
#include "AnimationManager.h"
#include "EffectConst.h"

CHitEffects::CHitEffects()
{
	LoadAnimation();
}

void CHitEffects::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(ENEMY_DAMAGED_EFFECT, animationManager->Get("ani-enemy-damaged"));
	AddAnimation(FIRE_BALL_DAMAGED_EFFECT, animationManager->Get("ani-enemy-damaged"));
}
