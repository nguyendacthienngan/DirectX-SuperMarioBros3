#include "EmptyBlock.h"
#include "AnimationManager.h"

CEmptyBlock::CEmptyBlock()
{
	LoadAnimation();
	SetState("EMPTY");
	isEnabled = true;
}

void CEmptyBlock::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("EMPTY", animations->Get("ani-empty-block"));
}
