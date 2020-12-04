#include "RedGoomba.h"
#include "AnimationManager.h"
CRedGoomba::CRedGoomba()
{
	LoadAnimation();
}

void CRedGoomba::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(GOOMBA_STATE_WALK, animationManager->Clone("ani-red-goomba-walk"));
	AddAnimation(GOOMBA_STATE_DIE, animationManager->Clone("ani-red-goomba-die"));
	AddAnimation(GOOMBA_STATE_IDLE, animationManager->Clone("ani-red-goomba-idle"));
	SetState(GOOMBA_STATE_WALK);
}
