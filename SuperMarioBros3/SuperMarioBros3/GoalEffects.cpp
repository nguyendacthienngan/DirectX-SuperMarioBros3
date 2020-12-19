#include "GoalEffects.h"
#include "AnimationManager.h"
#include "GoalFXConst.h"
#include "UICamera.h"
CGoalEffects::CGoalEffects()
{
	LoadAnimation();
	SetState("MUSHROOM");
	isEnabled = true;
}

void CGoalEffects::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("FLOWER", animationManager->Get("ani-goal-flower"));
	AddAnimation("STAR", animationManager->Get("ani-goal-star"));
	AddAnimation("MUSHROOM", animationManager->Get("ani-goal-mushroom"));
}

void CGoalEffects::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	// Update bay lên trời xong rồi gọi hàm GoalRoulette bên UiCamera và truyền cái state hiện có và tự disable
	transform.position.y -= GOAL_FX_VELOCITY * dt;
	if (transform.position.y < cam->GetPositionCam().y)
	{
		if (uiCam != NULL)
		{
			auto uiCamera = static_cast<CUICamera*>(uiCam);
			uiCamera->OnGoalRoulette(currentState);
			isEnabled = false; // Nên hong?
		}
	}
}