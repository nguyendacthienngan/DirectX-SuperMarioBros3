#include "Grass.h"
#include "AnimationManager.h"
#include "WorldItemConst.h"
CGrass::CGrass()
{
	LoadAnimation();
	SetState(GRASS_STATE_ANIMATION);
	isEnabled = true;
	physiscBody->SetDynamic(false);
}

void CGrass::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(GRASS_STATE_ANIMATION, animationManager->Get("ani-tree"));
}
