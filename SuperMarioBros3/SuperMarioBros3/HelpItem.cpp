#include "HelpItem.h"
#include "AnimationManager.h"
#include "WorldItemConst.h"
#include "Ultis.h"

CHelpItem::CHelpItem()
{
	LoadAnimation();
	SetState(HELP_STATE_ANIMATION);
	isEnabled = true;
	physiscBody->SetDynamic(false);
	timeStartChangeAlpha = 0;
	timeToChangeAlpha = 400;
	timeStopChangeAlpha = 0;
	countChangeAlpha = 0;
}

void CHelpItem::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(HELP_STATE_ANIMATION, animationManager->Get("ani-help-bubble"));
}

void CHelpItem::Render(CCamera* cam, int alpha)
{
	if (timeStartChangeAlpha != 0 && GetTickCount64() - timeStartChangeAlpha > timeToChangeAlpha)
	{
		alpha = 0;
		countChangeAlpha++;
	}
	if (alpha == 0 && countChangeAlpha == 1)
		timeStopChangeAlpha = GetTickCount64();

	DebugOut(L"TimeStopChangeAlpha %d \n", timeStopChangeAlpha);
	if (timeStopChangeAlpha != 0 && GetTickCount64() - timeStopChangeAlpha > timeToChangeAlpha)
	{
		DebugOut(L"AAAA\n");
		timeStartChangeAlpha = 0;
		timeStopChangeAlpha = 0;
		countChangeAlpha = 0;
	}
	CGameObject::Render(cam, alpha);
	if (timeStartChangeAlpha == 0)
		timeStartChangeAlpha = GetTickCount64();

}
