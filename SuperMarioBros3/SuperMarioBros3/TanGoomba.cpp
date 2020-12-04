#include "TanGoomba.h"
#include "AnimationManager.h"

CTanGoomba::CTanGoomba()
{
	LoadAnimation();
}


void CTanGoomba::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(GOOMBA_STATE_WALK, animationManager->Clone("ani-goomba-walk"));
	AddAnimation(GOOMBA_STATE_DIE, animationManager->Clone("ani-goomba-die"));
	AddAnimation(GOOMBA_STATE_IDLE, animationManager->Clone("ani-goomba-idle"));
	SetState(GOOMBA_STATE_WALK);
}

