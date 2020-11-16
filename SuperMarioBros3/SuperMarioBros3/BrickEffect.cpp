#include "BrickEffect.h"
#include "AnimationManager.h"
CBrickEffect::CBrickEffect()
{
	LoadAnimation();
	SetState("BRICK");
}

void CBrickEffect::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("BRICK", animations->Get("ani-brick"));
	
}
