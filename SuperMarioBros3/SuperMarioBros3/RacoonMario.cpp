#include "RacoonMario.h"
#include "MarioConst.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "MarioStateSet.h"
CRacoonMario::CRacoonMario()
{
	CMario::Init();
	marioStateTag = MarioStates::RacoonMario;
	canAttackContinious = true;
	canCrouch = true;
	canAttack = true;
	isJumpAttack = false;
	isAttackContinious = false;
	CRacoonMario::Init();
	CRacoonMario::LoadAnimation();
}

void CRacoonMario::Init()
{
	SetState(MARIO_STATE_IDLE);
}

void CRacoonMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();

	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-raccoon-mario-idle"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-raccoon-mario-walk"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-raccoon-mario-walk"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-raccoon-mario-jump"));
	AddAnimation(MARIO_STATE_FLY, animationManager->Get("ani-raccoon-mario-jump"));
	AddAnimation(MARIO_STATE_FULLFLY, animationManager->Get("ani-raccoon-mario-fly"));
	AddAnimation(MARIO_STATE_FLOAT, animationManager->Get("ani-raccoon-mario-float"));
	AddAnimation(MARIO_STATE_FALL, animationManager->Get("ani-raccoon-mario-fall"));
	AddAnimation(MARIO_STATE_SKID, animationManager->Get("ani-raccoon-mario-skid"));
	AddAnimation(MARIO_STATE_CROUCH, animationManager->Get("ani-raccoon-mario-crouch"));
	AddAnimation(MARIO_STATE_ATTACK, animationManager->Get("ani-raccoon-mario-spin"), false);
	AddAnimation(MARIO_STATE_JUMP_ATTACK, animationManager->Get("ani-raccoon-mario-spin"), false);
}

void CRacoonMario::EndAnimation()
{
	if (currentState.compare(MARIO_STATE_ATTACK) == 0)
	{
		if (isAttackContinious == false)
		{
			isAttack = false;
			isJumpAttack = false;
			if (animations.find(lastState) == animations.end()) // Không kiếm được last state trong animation, đồng nghĩa với việc last state chưa được khởi tạo, còn nếu đc khởi tạo rồi thì mình set state theo cái state trước đó
				lastState = MARIO_STATE_IDLE;
			SetState(lastState);
		}
	
	}
}

void CRacoonMario::Update(DWORD dt, CCamera* cam)
{
	auto keyboard = CKeyboardManager::GetInstance();
	CMario::Update(dt, cam);
	if (isAttack == true) 
	{
		currentPhysicsState.move = MoveOnGroundStates::Attack;
		if (keyboard->GetKeyStateDown(DIK_Z))
		{
			isAttackContinious = true;
		}
		if (isOnGround == false || isJump == true)
		{
			// S + Z
			currentPhysicsState.move = MoveOnGroundStates::JumpAttack;
			isJumpAttack = true;
		}
		if (currentPhysicsState.jump == JumpOnAirStates::Fall && currentPhysicsState.move == MoveOnGroundStates::JumpAttack)
		{
			currentPhysicsState.move = MoveOnGroundStates::Idle;
			isAttack = false;
			isJumpAttack = false;
		}
	}

	// Bay
	// Set Gravity = 0 để bé cáo bay thỏa thích trên trời, đến max time (4s) rồi thì hạ xuống từ từ
	// Lúc bay, ta sẽ set abs(vel.x), abs(vel.y) tăng

}

void CRacoonMario::OnKeyUp(int KeyCode)
{
	CMario::OnKeyUp(KeyCode);
	if (KeyCode == DIK_Z)
	{
		isAttackContinious = false;
	}
}

CRacoonMario::~CRacoonMario()
{
	CGameObject::~CGameObject();

}
