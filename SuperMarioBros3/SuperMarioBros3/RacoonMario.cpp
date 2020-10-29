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
	timeToFly = 2000;
	timeToKeyFlyDown = 200;
	lastFlyTime = 0;
	lastKeyFlyDown = 0;
	isFly = false;
	moreFlyPower = false;
	feverState = -1;
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
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-raccoon-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-raccoon-mario-walk"));
	AddAnimation(MARIO_STATE_HIGH_SPEED, animationManager->Get("ani-raccoon-mario-speed-up"));

	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-raccoon-mario-jump"));
	AddAnimation(MARIO_STATE_FLY, animationManager->Get("ani-raccoon-mario-fly"));
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


	auto velocity = physiscBody->GetVelocity();
	auto sign = physiscBody->GetNormal().x;
	if (currentPhysicsState.jump == JumpOnAirStates::Fly)
	{
		if (isOnGround == true)
			currentPhysicsState.jump = JumpOnAirStates::Stand;
	}
	if (GetTickCount64() - lastKeyFlyDown > timeToKeyFlyDown && lastKeyFlyDown != 0 && moreFlyPower == true) // có thể ở đây sai
	{
		DebugOut(L"STOP FLY \n!!!");
		moreFlyPower = false;
		if (canFly == true)
			physiscBody->SetGravity(MARIO_GRAVITY / 2);
	}
	//if (isFly == true)
	//{
	//	currentPhysicsState.jump = JumpOnAirStates::Fly;
	//	physiscBody->SetGravity(0.0f);
	//}
	
}

void CRacoonMario::OnKeyDown(int KeyCode)
{
	CMario::OnKeyDown(KeyCode);
	
	if (KeyCode == DIK_S)
	{
		if (canFly == true && isFly == false)
			lastFlyTime = GetTickCount64();
		if (GetTickCount() - lastFlyTime > timeToFly && lastFlyTime != 0 && isFly == true) //***** CẦN CHECK LẠI
		{
			// chưa làm rớt chầm chậm đc
			isFly = false;
			physiscBody->SetGravity(MARIO_GRAVITY);
			if (isOnGround == false)
				currentPhysicsState.jump = JumpOnAirStates::Fall;
			else
				currentPhysicsState.jump = JumpOnAirStates::Stand;
			pMeterCounting = 0;
			canFly = false; 
			lastFlyTime = 0;
			feverState = -1;
			return;
		}
		if (canFly == true)
		{
			// Vô đc mà tại sao nó k thể trừ tiếp đc? Có thể là do lúc này gravity khác nhau
			// Nên có trừ cx k đáng kể
			// Đến khi xét va chạm với mặt đất => gravity thay đổi => Mới bay đc

			// Vẫn chưa nhấn liên tục dược => sau khi bị moreflypower = false thì phải đợi rớt xuống đất mới có thể fly lại đc

			moreFlyPower = true;
			auto velocity = physiscBody->GetVelocity();
			auto sign = physiscBody->GetNormal().x;
			DebugOut(L"To be fly ~\n");
			isFly = true;

			currentPhysicsState.jump = JumpOnAirStates::Fly;
			velocity.y -= MARIO_FLY_FORCE * dt; 
			velocity.x += sign * MARIO_FLY_FORCE;
			physiscBody->SetGravity(0.0f);
			physiscBody->SetVelocity(velocity);
			lastKeyFlyDown = GetTickCount64();
		}
		
	}

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
