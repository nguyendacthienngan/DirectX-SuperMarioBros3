#include "VenusGreen.h"
#include "VenusConst.h"
#include "AnimationManager.h"
CVenusGreen::CVenusGreen()
{
	CVenusGreen::LoadAnimation();
	collisionBoxs->at(0)->SetSizeBox(VENUS_GREEN_BBOX);
}

void CVenusGreen::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(VENUS_STATE_HEAD_UP, animationManager->Get("ani-green-venus-fire-trap-headup"));
	AddAnimation(VENUS_STATE_HEAD_DOWN, animationManager->Get("ani-green-venus-fire-trap-headdown"));
}
