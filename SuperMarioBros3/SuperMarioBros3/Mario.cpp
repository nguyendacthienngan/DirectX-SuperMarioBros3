#include "Mario.h"

#include "AnimationManager.h"

CMario::CMario()
{
	LoadAnimation();
}

void CMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("Run",animationManager->Clone("ani-small-mario-run"));

	// Có thể không chạy đc CLone
	this->SetState("Run");
}

CMario::~CMario()
{
}
