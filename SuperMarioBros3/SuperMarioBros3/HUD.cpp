#include "HUD.h"
#include "AnimationManager.h"

CHUD::CHUD()
{
	LoadAnimation();
	SetState("HUD");
	isEnabled = true;
}

void CHUD::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("HUD", animationManager->Get("ani-hud"));
}
