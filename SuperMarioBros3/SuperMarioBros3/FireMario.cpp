#include "FireMario.h"
#include "AnimationManager.h"
#include "MarioConst.h"
CFireMario::CFireMario()
{
	CMario::Init();
	SetTag(GameObjectTags::Player);
	Init();
	isThrowingFireBall = false;
	lastState = currentState;
	canCrouch = true;
}

void CFireMario::Init()
{
	LoadAnimation();
	this->SetState(MARIO_STATE_IDLE); // Để tên đồng nhất với animation
}

void CFireMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-fire-mario-high-speed"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-fire-mario-walk"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-fire-mario-jump"));
	AddAnimation(MARIO_STATE_FLY, animationManager->Get("ani-fire-mario-high-jump"));
	AddAnimation(MARIO_STATE_FALL, animationManager->Get("ani-fire-mario-fall"));
	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-fire-mario-idle"));
	AddAnimation(MARIO_STATE_SKID, animationManager->Get("ani-fire-mario-skid"));
	AddAnimation(MARIO_STATE_CROUCH, animationManager->Get("ani-fire-mario-crouch"));
	AddAnimation(MARIO_STATE_ATTACK, animationManager->Get("ani-fire-mario-throw"), false);
}

void CFireMario::Render(CCamera* cam)
{
	auto ani = GetAnimationByState(currentState);

	if (isThrowingFireBall == true)
	{
		if (currentState.compare(MARIO_STATE_ATTACK) != 0) // Chưa set state Attack
			SetState(MARIO_STATE_ATTACK);
		ani->SetSpeedMultiplier(2.0f);
		return;
	}
	CMario::Render(cam);

	// ************ Còn phải check lại
	//if (currentState.compare(MARIO_STATE_ATTACK) == 0) // đã set attack
	//{
	//	isThrowingFireBall = false;
	//	if (animations.find(lastState) == animations.end())
	//		lastState = MARIO_STATE_IDLE;
	//	SetState(lastState);
	//}
	
}

//void CFireMario::OnKeyDown(int KeyCode)
//{
//	CMario::OnKeyDown(KeyCode);
//
//	//// ATTACK / THROW FIRE BALL
//	//if (KeyCode == DIK_A && isThrowingFireBall == false)
//	//	isThrowingFireBall = true;
//
//}
