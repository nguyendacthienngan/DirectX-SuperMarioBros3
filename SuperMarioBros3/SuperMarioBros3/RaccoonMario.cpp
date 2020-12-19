#include "RaccoonMario.h"
#include "MarioConst.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "MarioStateSet.h"
#include "PMeterConst.h"
#include "Game.h"
CRaccoonMario::CRaccoonMario()
{
	CMario::Init();
	collisionBoxs->at(0)->SetName("Raccoon-Mario");
	marioStateTag = MarioStates::RacoonMario;
	raccoonTailBox = new CRaccoonTailBox();
	CRaccoonMario::Init();
	CRaccoonMario::LoadAnimation();
}

void CRaccoonMario::Init()
{
	SetState(MARIO_STATE_IDLE);
	canAttackContinious = true;
	canCrouch = true;
	canAttack = true;
	isJumpAttack = false;
	isAttackContinious = false;

#pragma region Các biến flag để xét việc Fly
	timeToFly = FLYING_TIME;
	timeToKeyFlyDown = TIME_TO_PRESS_S_TO_FLY;
	lastFlyTime = 0;
	lastKeyFlyDown = 0;
	isFly = false;
	flyDown = false;
	moreFlyPower = false;
#pragma endregion

#pragma region Các biến flag để xét việc Float
	timeToFloat = FLOATING_TIME;
	timeToKeyFloatDown = TIME_TO_PRESS_S_TO_FLOAT;
	moreFloatPower = false;
	feverState = -1;
	canFloat = false;
	isFloat = false;
	lastFloatTime = 0;
#pragma endregion
	
#pragma region  Các biến flag để xét việc Attack (Quay đuôi)
	timeToAttack = ATTACKING_TIME;
	beginAttackTime = -1;
	beginAttackTail = false;
#pragma endregion

}

void CRaccoonMario::LoadAnimation()
{
	CMario::LoadAnimation();

	auto animationManager = CAnimationManager::GetInstance();

	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-raccoon-mario-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-raccoon-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-raccoon-mario-walk"));
	AddAnimation(MARIO_STATE_HIGH_SPEED, animationManager->Get("ani-raccoon-mario-speed-up"));

	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-raccoon-mario-jump"));
	AddAnimation(MARIO_STATE_FLY, animationManager->Get("ani-raccoon-mario-fly"));
	AddAnimation(MARIO_STATE_FLOAT, animationManager->Get("ani-raccoon-mario-float"), false);
	AddAnimation(MARIO_STATE_FALL, animationManager->Get("ani-raccoon-mario-fall"));
	AddAnimation(MARIO_STATE_SKID, animationManager->Get("ani-raccoon-mario-skid"));
	AddAnimation(MARIO_STATE_CROUCH, animationManager->Get("ani-raccoon-mario-crouch"));
	AddAnimation(MARIO_STATE_ATTACK, animationManager->Get("ani-raccoon-mario-spin"), false);
	AddAnimation(MARIO_STATE_JUMP_ATTACK, animationManager->Get("ani-raccoon-mario-spin"), false);
	AddAnimation(MARIO_STATE_HOLD_MOVE, animationManager->Get("ani-raccoon-mario-hold"));
	AddAnimation(MARIO_STATE_HOLD_IDLE, animationManager->Get("ani-raccoon-mario-hold-idle"));
	AddAnimation(MARIO_STATE_KICK, animationManager->Get("ani-raccoon-mario-kick"), false);
	AddAnimation(MARIO_STATE_IDLE_FRONT, animationManager->Get("ani-raccoon-mario-idle-front"));
}

void CRaccoonMario::EndAnimation()
{
	CMario::EndAnimation();

	if (currentState.compare(MARIO_STATE_ATTACK) == 0)
	{
		if (isAttackContinious == false)
		{
			isAttack = false;
			isJumpAttack = false;
			raccoonTailBox->Enable(false);
			beginAttackTail = false;

			if (animations.find(lastState) == animations.end()) // Không kiếm được last state trong animation, đồng nghĩa với việc last state chưa được khởi tạo, còn nếu đc khởi tạo rồi thì mình set state theo cái state trước đó
				lastState = MARIO_STATE_IDLE;
			SetState(lastState);
		}
	}

	if (currentState.compare(MARIO_STATE_FLOAT) == 0)
	{
		isFloat = false;
		if (animations.find(lastState) == animations.end())
			lastState = MARIO_STATE_IDLE;
		SetState(lastState);
	}
}

void CRaccoonMario::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto keyboard = CKeyboardManager::GetInstance();
	CMario::Update(dt, cam, uiCam);
	if (isAttack == true && isDamaged == false) 
	{
		// Nếu vừa ấn attack cái enable cái đuôi là sai vì tới frame thứ 3 của Attack Animation mới là cái đuôi giơ ra
		// Nên mình sẽ set cái time phù hợp để xét va chạm đuôi đúng
		if (beginAttackTime != -1)
			beginAttackTime += CGame::GetInstance()->GetDeltaTime() * CGame::GetTimeScale();

		if (beginAttackTime > ATTACKING_TIME && beginAttackTime != -1)
		{
			raccoonTailBox->Enable(true);
			beginAttackTail = true;
		}
#pragma region Xử lý việc quay đuôi với phím tắt
		currentPhysicsState.move = MoveOnGroundStates::Attack;
		if (keyboard->GetKeyStateDown(DIK_Z))
		{
			// Z giữ lâu
			isAttackContinious = true;
		}
		if (isOnGround == false || isJump == true || canLowJumpContinous == true)
		{
			// S + Z
			// X + Z?
			currentPhysicsState.move = MoveOnGroundStates::JumpAttack;
			isJumpAttack = true;
		}
		if (currentPhysicsState.jump == JumpOnAirStates::Fall && currentPhysicsState.move == MoveOnGroundStates::JumpAttack)
		{
			currentPhysicsState.move = MoveOnGroundStates::Idle;
			isAttack = false;
			isJumpAttack = false;
			raccoonTailBox->Enable(false);
		}
#pragma endregion

	}
	if (isAttack == false)
		beginAttackTime = -1;
	// Bay
	// Set Gravity = 0 để bé cáo bay thỏa thích trên trời, đến max time (4s) rồi thì hạ xuống từ từ
	//DebugOut(L"pMeterCounting %f \n", pMeterCounting);
	//DebugOut(L"feverState %d \n", feverState);
	auto velocity = physiscBody->GetVelocity();
	auto sign = physiscBody->GetNormal().x;

#pragma region Xử lý việc bay của Mario
	
	if (currentPhysicsState.jump == JumpOnAirStates::Fly)
	{
		// Xử lý việc khi đang bay mà chạm đất
		if (isOnGround == true)
			currentPhysicsState.jump = JumpOnAirStates::Stand;
	}
	if (canFly == true && currentPhysicsState.jump == JumpOnAirStates::Fall && isOnGround == false && moreFlyPower == true)
	{
		// Xử lý việc thả nút xong ấn lại liền cho Fly
		currentPhysicsState.jump = JumpOnAirStates::Fly;
		physiscBody->SetGravity(0.0f);
		lastKeyFlyDown = GetTickCount64();
		isFly = true;
	}

	if (GetTickCount64() - lastKeyFlyDown > timeToKeyFlyDown && lastKeyFlyDown != 0 && moreFlyPower == true)
	{
		// Sau khi thả nút S
		moreFlyPower = false;
		if (canFly == true)
			physiscBody->SetGravity(MARIO_GRAVITY / 2);
		flyDown = true;
	}
	
#pragma region FEVER STATE
	
	if (uiCamera != NULL)
	{
		// 4: Bắt đầu bước vào tính pMetercounting
		if (pMeterCounting >= 0 && pMeterCounting <= PMETER_MAX)
			if (uiCamera->GetHUD()->GetPMeter()->GetFeverState() < 5)
				uiCamera->GetHUD()->GetPMeter()->SetFeverState(4);
		
		// 5: Cất cánh bayyyy
		if (canFly == true || isFly == true)
		{
			uiCamera->GetHUD()->GetPMeter()->SetFeverState(5);
			uiCamera->GetHUD()->GetPMeter()->SetCanFly(true);
		}

		// 6: Hạ cánh an toàn
		if (GetTickCount64() - lastFlyTime > timeToFly && lastFlyTime != 0 && isFly == true)
		{
			uiCamera->GetHUD()->GetPMeter()->SetFeverState(-1);
			uiCamera->GetHUD()->GetPMeter()->SetCanFly(false);
			pMeterCounting = 0.0f;
		}

		uiCamera->GetHUD()->GetPMeter()->SetIsRaccoonMario(true);
	}

#pragma endregion

#pragma endregion

	

#pragma region Xử lý việc sau khi bay mà quẫy đuôi bay chậm

	if ((GetTickCount64() - lastFloatTime > timeToFloat || isOnGround == true) && canFly == false)
	{
		canFloat = false;
		isFloat = false;
		moreFloatPower = false;
		physiscBody->SetGravity(MARIO_GRAVITY);
	}
	if (currentPhysicsState.jump == JumpOnAirStates::Fall && isOnGround == false && moreFlyPower == true)
	{
		// Xử lý việc thả nút xong ấn lại liền cho Float
		currentPhysicsState.jump = JumpOnAirStates::Float;
		physiscBody->SetGravity(0.0f);
		isFloat = true;
		auto velo = physiscBody->GetVelocity();
		velo.y = RACCOON_FLOAT_VELOCITY;

		physiscBody->SetVelocity(velo);
	}

	if (GetTickCount64() - lastKeyFloatDown > timeToKeyFloatDown && lastKeyFloatDown != 0 && moreFloatPower == true)
	{
		moreFloatPower = false;
		physiscBody->SetGravity(MARIO_GRAVITY);

	}
	// Xử lý việc khi bắt đầu nhấn S cho vẫy đuôi
	if (isFloat == true && moreFloatPower == true)
		//if (isFloat == true && moreFloatPower == true && canFloat == true)
	{
		physiscBody->SetGravity(0.0f);
		auto velo = physiscBody->GetVelocity();
		velo.y = RACCOON_FLOAT_VELOCITY;

		physiscBody->SetVelocity(velo);
		currentPhysicsState.jump = JumpOnAirStates::Float;
	}
	if (currentPhysicsState.jump == JumpOnAirStates::Float && isOnGround == true)
		currentPhysicsState.jump = JumpOnAirStates::Stand;
		
#pragma endregion

	// Set vị trí của TailBox ngay trước mặt Mario (overlap 1 khoảng với box của Mario)
	auto tailPosition = transform.position;
	auto normal = physiscBody->GetNormal();
	tailPosition.x += SUPER_MARIO_BBOX.x*0.8f*normal.x ;
	raccoonTailBox->SetPosition(tailPosition);
}

void CRaccoonMario::AddObjectToScene(LPScene scene)
{
	scene->AddObject(raccoonTailBox);
}

void CRaccoonMario::OnKeyDown(int KeyCode)
{
	CMario::OnKeyDown(KeyCode);
	if (KeyCode == DIK_S)
	{
#pragma region Xử lý việc bay của Mario
		// FLY
		if (canFly == true && isFly == false)
			lastFlyTime = GetTickCount64();
		if (GetTickCount() - lastFlyTime > timeToFly && lastFlyTime != 0 && isFly == true) 
		{
			ResetValueFly();
		}
		if (canFly == true)
		{
			moreFlyPower = true;
			auto velocity = physiscBody->GetVelocity();
			auto sign = physiscBody->GetNormal().x;
			isFly = true;

			currentPhysicsState.jump = JumpOnAirStates::Fly;
			velocity.y = RACCOON_FLY_VELOCITY;
			velocity.x += sign * MARIO_FLY_FORCE*dt;
			physiscBody->SetGravity(0.0f);
			physiscBody->SetVelocity(velocity);
			lastKeyFlyDown = GetTickCount64();
			flyDown = false;
		}
#pragma endregion

#pragma region Xử lý việc sau khi bay mà quẫy đuôi bay chậm

		//// FLOAT
		if (isFloat == false)
			lastFloatTime = GetTickCount64();

		if (canFly == false && currentPhysicsState.jump == JumpOnAirStates::Fall && isOnGround == false)
		{
			isFloat = true;
			lastKeyFloatDown = GetTickCount64();
			moreFloatPower = true;
			physiscBody->SetGravity(0.0f);
		}
#pragma endregion

	}
}

void CRaccoonMario::OnKeyUp(int KeyCode)
{
	CMario::OnKeyUp(KeyCode);
	if (KeyCode == DIK_Z)
	{
		isAttackContinious = false;
	}
}

CRaccoonTailBox* CRaccoonMario::GetRaccoonTailBox()
{
	return raccoonTailBox;
}

void CRaccoonMario::SetRaccoonTailBox(CRaccoonTailBox* racTailBox)
{
	this->raccoonTailBox = racTailBox;
}

void CRaccoonMario::ResetValueAttack()
{
	currentPhysicsState.move = MoveOnGroundStates::Idle;
	isAttack = false;
	isJumpAttack = false;
	raccoonTailBox->Enable(false);
}

void CRaccoonMario::ResetValueFly()
{
	if (uiCamera != NULL)
	{
		uiCamera->GetHUD()->GetPMeter()->SetFeverState(-1);
		uiCamera->GetHUD()->GetPMeter()->SetCanFly(false);
	}
	isFly = false;
	physiscBody->SetGravity(MARIO_GRAVITY);
	if (isOnGround == false)
		currentPhysicsState.jump = JumpOnAirStates::Fall;
	else
		currentPhysicsState.jump = JumpOnAirStates::Stand;
	canFly = false;
	lastFlyTime = 0;
	canFloat = true;
	pMeterCounting = 0.0f;
	feverState = -1;
}

bool CRaccoonMario::IsFly()
{
	return isFly;
}

CRaccoonMario::~CRaccoonMario()
{
	CGameObject::~CGameObject();

}
