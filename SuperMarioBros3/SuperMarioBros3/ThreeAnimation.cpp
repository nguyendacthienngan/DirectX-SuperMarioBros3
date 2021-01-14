#include "ThreeAnimation.h"
#include "AnimationManager.h"
CThreeAnimation::CThreeAnimation()
{
	LoadAnimation();
	SetState("THREE");
	isEnabled = true;
}

void CThreeAnimation::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("THREE", animationManager->Get("ani-three"));
}
