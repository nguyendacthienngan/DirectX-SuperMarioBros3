#include "VenusRed.h"
#include "VenusConst.h"
#include "AnimationManager.h"
CVenusRed::CVenusRed()
{
	CVenusRed::LoadAnimation();
	SetState(VENUS_STATE_HEAD_UP);
}

void CVenusRed::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(VENUS_STATE_HEAD_UP, animationManager->Clone("ani-red-venus-fire-trap-headup"));
	AddAnimation(VENUS_STATE_HEAD_DOWN, animationManager->Clone("ani-red-venus-fire-trap-headdown"));
}
