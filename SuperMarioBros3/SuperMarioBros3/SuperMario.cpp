#include "SuperMario.h"
#include "AnimationManager.h"
#include "MarioConst.h"
#include "Ultis.h"
CSuperMario::CSuperMario()
{
	CMario::Init();
	CSuperMario::LoadAnimation();
	CSuperMario::Init();
	CMario::InitProperties();
	canCrouch = true;
	canAttack = false;
}
void CSuperMario::Init()
{
	this->SetState(MARIO_STATE_IDLE); // Để tên đồng nhất với animation
}
void CSuperMario::LoadAnimation()
{
	DebugOut(L"Init Super Mario \n");
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-big-mario-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-big-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-big-mario-run"));
	AddAnimation(MARIO_STATE_HIGH_SPEED, animationManager->Get("ani-big-mario-high-speed"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-big-mario-jump"));
	AddAnimation(MARIO_STATE_CROUCH, animationManager->Get("ani-big-mario-crouch"));
	AddAnimation(MARIO_STATE_SKID, animationManager->Get("ani-big-mario-skid"));
	AddAnimation(MARIO_STATE_FALL, animationManager->Get("ani-big-mario-fall"));
}
