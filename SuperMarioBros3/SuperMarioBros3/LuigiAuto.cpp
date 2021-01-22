#include "LuigiAuto.h"
#include "AnimationManager.h"
#include "MarioConst.h"
CLuigiAuto::CLuigiAuto()
{
	CMario::Init();
	collisionBoxs->at(0)->SetName("Super-Mario");
	CLuigiAuto::LoadAnimation();
	SetState(MARIO_STATE_IDLE);
	marioStateTag = MarioStates::SuperMario;

}

void CLuigiAuto::LoadAnimation()
{
	CMario::LoadAnimation();
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-big-luigi-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-big-luigi-walk"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-big-luigi-jump"));
	AddAnimation(MARIO_STATE_FALL, animationManager->Get("ani-big-luigi-fall"));
}
