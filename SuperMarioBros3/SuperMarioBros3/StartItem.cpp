#include "StartItem.h"
#include "AnimationManager.h"
#include "WorldItemConst.h"
#include "Ultis.h"

CStartItem::CStartItem()
{
	LoadAnimation();
	SetState(START_STATE_ANIMATION);
	isEnabled = true;
	physiscBody->SetDynamic(false);
}

void CStartItem::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(START_STATE_ANIMATION, animationManager->Get("ani-start"));
}
