#include "Piranha.h"
#include "AnimationManager.h"
#include "PiranhaConst.h"

CPiranha::CPiranha()
{
	LoadAnimation();
	Init();
}

void CPiranha::Init()
{
	isEnabled = true;
	SetState(PIRANHA_STATE_ATTACK);
}

void CPiranha::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(PIRANHA_STATE_ATTACK, animationManager->Clone("ani-green-piranha-plant-attack"));
}
