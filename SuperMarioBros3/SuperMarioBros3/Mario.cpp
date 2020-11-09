#include "Mario.h"

#include "AnimationManager.h"
#include "Game.h"
#include "Ultis.h"
#include "MarioConst.h"
#include "Const.h"
#include "Sprite.h"

#include "CollisionBox.h"
#include "Enemy.h"
#include "KoopaShell.h"
#include "KoopaConst.h"

#include <cstdlib>
#include <cctype> 
#include <string>
#include "Koopa.h"
#include "MarioCollisionBox.h"
using namespace std;

CMario::CMario()
{
	LoadAnimation();
	InitProperties();
	tag = GameObjectTags::Player;

}

void CMario::Init()
{
	this->SetState(MARIO_STATE_IDLE); 
	CMarioCollisionBox* collisionBox = new CMarioCollisionBox();
	collisionBox->SetSizeBox(SUPER_MARIO_BBOX); // Big
	collisionBox->SetPosition(D3DXVECTOR2(0.0f, 0.0f)); // Local Position
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Mario");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);
}

void CMario::InitProperties()
{
	this->physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	this->physiscBody->SetDynamic(true); // có chuyển động
	this->physiscBody->SetGravity(MARIO_GRAVITY);

	currentPhysicsState =
	{
		MoveOnGroundStates::Idle,
		JumpOnAirStates::Stand
	};
	targetVelocity.x = 0.0f;
	targetVelocity.y = 0.0f;

	previousNormal = physiscBody->GetNormal();
	isEnabled = true;
	isOnGround = false;
	canHighJump = false;
	isJump = false;
	isSkid = false;
	isAttack = false;
	canFly = false;
	isFly = false;
	stopBounce = false;
	isKick = false;
	bounceAfterJumpOnEnemy = false;
	feverTime = MARIO_FEVER_TIME;
	lastFeverTime = 0;
	feverState = 0;
	pMeterCounting = 0.0f;
	beforeJumpPosition = 0.0f;
	startDeflectTime = 0;
	this->SetScale(D3DXVECTOR2(1.0f, 1.0f));

}

void CMario::LoadAnimation()
{
}

void CMario::EndAnimation()
{
	if (currentState.compare(MARIO_STATE_KICK) == 0)
	{
		isKick = false;
	}
}

void CMario::Update(DWORD dt, CCamera* cam)
{
	CGameObject::Update(dt, cam);
	auto keyboard = CKeyboardManager::GetInstance();
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	previousVelocity = velocity;
	float acceleration = 0.0f;

	D3DXVECTOR2 drag = physiscBody->GetDragForce();

	// Horizontal Movement: Walk, Run, Idle
	if (keyboard->GetKeyStateDown(DIK_RIGHT) || keyboard->GetKeyStateDown(DIK_LEFT))
	{
		// Nhấn nút A chạy ! RUN
		if (keyboard->GetKeyStateDown(DIK_A))
		{
			currentPhysicsState.move = MoveOnGroundStates::Run;
			acceleration = MARIO_RUNNING_ACCELERATION;
			targetVelocity.x = MARIO_RUNNING_SPEED;
		}
		else
		{
			// Không chạy -> Đi bộ bình thường: WALK
			currentPhysicsState.move = MoveOnGroundStates::Walk;
			acceleration = MARIO_WALKING_ACCELERATION;
			targetVelocity.x = MARIO_WALKING_SPEED;
		}

		normal.x = (keyboard->GetKeyStateDown(DIK_RIGHT)) ? 1 : -1;
		physiscBody->SetNormal(normal);
		physiscBody->SetAcceleration(acceleration * normal.x);

		if (abs(velocity.x) < targetVelocity.x)
			velocity.x += physiscBody->GetAcceleration() * dt;

		FrictionProcess(velocity.x, dt); // Kèm lực ma sát kéo lại vận tốc
		physiscBody->SetVelocity(velocity);

		if (abs(velocity.x) > MARIO_RUNNING_SPEED * 0.95f)
			currentPhysicsState.move = MoveOnGroundStates::HighSpeed;

		SkidProcess(velocity);
		if (previousVelocity.x * velocity.x <= 0)
			isSkid = false;
		if (isSkid == true)
			currentPhysicsState.move = MoveOnGroundStates::Skid;
	}
	else
	{
		FrictionProcess(velocity.x, dt);
		physiscBody->SetVelocity(velocity);

		if (velocity.x == 0)
			if (currentPhysicsState.move != MoveOnGroundStates::Idle)
				currentPhysicsState.move = MoveOnGroundStates::Idle;
		isSkid = false;
	}
	
#pragma region P-METER
	if (currentPhysicsState.move == MoveOnGroundStates::Run
		&& abs(velocity.x) > MARIO_RUNNING_SPEED * 0.15f
		&& pMeterCounting < PMETER_MAX + 1
		&& currentPhysicsState.jump == JumpOnAirStates::Stand
		&& feverState != 2)
	{
		if (feverState != -1)
			feverState = 1;
		pMeterCounting += PMETER_STEP * dt;
		if (pMeterCounting > PMETER_MAX + 1)
		{
			pMeterCounting = PMETER_MAX + 1;
		}
	}
	else if (feverState != 2 && feverState != -1) // nếu feverState đang = 1 mà k thỏa những điều kiện trên thì reset lại
		feverState = 0;

#pragma region FEVER STATE
	if (pMeterCounting >= PMETER_MAX && feverState == 0)
	{
		feverState = 2;
		lastFeverTime = GetTickCount();
	}
	else if (pMeterCounting > 0 && feverState == -1) // feverState = -1 là con raccoon
	{
		if (pMeterCounting >= PMETER_MAX)
		{
			canFly = true;
		}
	}
	if (feverState == 2)
	{
		pMeterCounting = PMETER_MAX; // giữ giá trị max 1 thời gian
		if (GetTickCount() - lastFeverTime > MARIO_FEVER_TIME)
		{
			feverState = 0;
			pMeterCounting = 0.0f;
		}
	}
#pragma endregion

#pragma endregion
	// Vertical Movement: Jump, High Jump, Super Jump

#pragma region STATE JUMP

// Điều kiện để có high-jump => Đã phải Jump (ấn 1 lần nút S) + giữ lâu tới target
// Để bé Mario k làm ninja thì phải kiểm tra OnCollisionEnter để coi ẻm có onGround hay chưa
// Điều kiện để có super-jump => Đã phải run tới mức cao nhất của p-meter => và nhấn S
// Xử lý super-jump tương đương high-jump nhưng tăng giới hạn khoảng cách lên cao hơn high-jump (và chuyển qua animation giơ 2 tay và ưỡn bụng)

// Đối với ấn nút X giữ lâu => Mario sẽ nhảy liên tục
// Ấn S giữ lâu thì chỉ nhảy cao hơn thôi
	if (keyboard->GetKeyStateDown(DIK_X) && isOnGround == true)
	{
		// Nhảy liên tục: Chỉ cần cung cấp dy < 0 và có gravity thì ta tạo cảm giác nó nhảy liên tục chuyển động đều

		if (canLowJumpContinous == true)
		{
			velocity.y -= MARIO_JUMP_SPEED_Y;
			isOnGround = false;
		}
		currentPhysicsState.jump = JumpOnAirStates::LowJump;
	}
	else if (currentPhysicsState.jump == JumpOnAirStates::LowJump)
	{
		if (velocity.y > 0) // Hướng xuống 
		{
			currentPhysicsState.jump = JumpOnAirStates::Fall;
		}
		else
			currentPhysicsState.jump = JumpOnAirStates::LowJump;
	}
	if (currentPhysicsState.jump == JumpOnAirStates::Jump && canLowJumpContinous == false && canHighJump == true)
	{
		if ( (keyboard->GetKeyStateDown(DIK_S) && isFly == false) || bounceAfterJumpOnEnemy == true)
		{
			float jumpMaxHeight;
			if ( (feverState == 2 && abs(velocity.x) > MARIO_RUNNING_SPEED * 0.85f) || bounceAfterJumpOnEnemy == true)
			{
				// SUPER JUMP
				jumpMaxHeight = MARIO_SUPER_JUMP_HEIGHT;
			}
			else
			{
				// HIGH - JUMP
				jumpMaxHeight = MARIO_HIGH_JUMP_HEIGHT;

			}
			if (abs(beforeJumpPosition) - abs(transform.position.y) <= jumpMaxHeight)
			{
				velocity.y = -MARIO_PUSH_FORCE;
			}
			else
			{
				// EndJump
				velocity.y = -MARIO_PUSH_FORCE / 2;
				canHighJump = false;
			}
			if (bounceAfterJumpOnEnemy == true)
			{
				bounceAfterJumpOnEnemy = false;
				canHighJump = false;
				stopBounce = true; // ****
			}

		}
	}
	if (velocity.y > 0)
	{
		currentPhysicsState.jump = JumpOnAirStates::Fall;
	}
	if (currentPhysicsState.jump == JumpOnAirStates::Fly) // Có thể lỗi ở đây
	{
		if (isOnGround == true)
			currentPhysicsState.jump = JumpOnAirStates::Stand;
		feverState = 0;
		pMeterCounting = 0;
	}
	if (currentPhysicsState.jump == JumpOnAirStates::Fall)
	{
		if (isOnGround == true)
			currentPhysicsState.jump = JumpOnAirStates::Stand;
		isJump = false;
	}


#pragma endregion

	physiscBody->SetVelocity(velocity);
	physiscBody->SetNormal(normal);


	if (canCrouch == true) // Small Mario k thể crouch
		CrouchProcess(keyboard);
	HoldProcess();
	if (isKick == true)
	{
		previousPhysicsState.move = currentPhysicsState.move;
		currentPhysicsState.move = MoveOnGroundStates::Kick;
	}
	if (currentPhysicsState.move == MoveOnGroundStates::Kick && isKick == false)
		currentPhysicsState.move = previousPhysicsState.move;
}

void CMario::Render(CCamera* cam)
{
	SetScale(D3DXVECTOR2(physiscBody->GetNormal().x, 1.0f));

#pragma region Update State

#pragma region Move On Ground
	switch (currentPhysicsState.move)
	{
		case MoveOnGroundStates::Idle:
		{
			if (isHold == true)
				SetState(MARIO_STATE_HOLD_IDLE);
			else
				SetState(MARIO_STATE_IDLE);
			break;
		}
		case MoveOnGroundStates::Walk:
		{
			if (isHold == true)
				SetState(MARIO_STATE_HOLD_MOVE);
			else
				SetState(MARIO_STATE_WALKING);
			break;
		}
		case MoveOnGroundStates::Run:
		{
			if (isHold == true)
				SetState(MARIO_STATE_HOLD_MOVE);
			else 
				SetState(MARIO_STATE_RUNNING);
			break;
		}
		case MoveOnGroundStates::Skid:
		{
			if (isHold == false)
			{
				auto normal = physiscBody->GetNormal();
				SetScale(D3DXVECTOR2(-normal.x, 1.0f));
				SetState(MARIO_STATE_SKID);
				break;
			}
			
		}
		case MoveOnGroundStates::Crouch:
		{
			if (isHold == false)
				SetState(MARIO_STATE_CROUCH);
			break;
		}
		case MoveOnGroundStates::HighSpeed:
		{
			if (isHold == false)
				SetState(MARIO_STATE_HIGH_SPEED);
			break;
		}
		case MoveOnGroundStates::Attack:
		{
			if (isHold == false) // Tạm thời
				SetState(MARIO_STATE_ATTACK);
			break;
		}
		case MoveOnGroundStates::JumpAttack:
		{
			if (isHold == false)
				SetState(MARIO_STATE_JUMP_ATTACK);
			break;
		}
		case MoveOnGroundStates::Kick:
		{
			DebugOut(L"KICK \n");
			if (isHold == false)
				SetState(MARIO_STATE_KICK);
			break;
		}
	}
#	pragma endregion

#pragma region Jump On Air
	if (currentPhysicsState.move != MoveOnGroundStates::JumpAttack && isHold == false)
	{
		switch (currentPhysicsState.jump)
		{
		case JumpOnAirStates::Jump: case JumpOnAirStates::HighJump:
		case JumpOnAirStates::LowJump:
		{
			SetState(MARIO_STATE_JUMP);
			break;
		}
		case JumpOnAirStates::Fall:
		{
			SetState(MARIO_STATE_FALL);
			break;
		}
		case JumpOnAirStates::Fly: //Riêng raccoon
		{
			SetState(MARIO_STATE_FLY);
			break;
		}
		case JumpOnAirStates::Float:
		{
			SetState(MARIO_STATE_FLOAT);
			break;
		}
		}
		if (feverState == 2 && isOnGround == false) // Các mario khác
		{
			SetState(MARIO_STATE_FLY);

		}
		
	}
	
#pragma endregion

#pragma endregion
	if (currentState != "IDLE")
		OutputDebugString(ToLPCWSTR("Current State " + currentState + "\n"));

	SetRelativePositionOnScreen(collisionBoxs->at(0)->GetPosition());
	CGameObject::Render(cam);
		
}

void CMario::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Solid || collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::GhostPlatform)
		{
			if (collisionEvent->ny < 0 && isOnGround == false)
			{
				isOnGround = true;
				DebugOut(L"OnGround\n");
			}
			if (collisionEvent->nx != 0) // vừa ấn nhảy vừa ấn qua trái phải
			{
				auto v = physiscBody->GetVelocity();
				physiscBody->SetVelocity(D3DXVECTOR2(0, v.y));
				pMeterCounting = 0;
			}
		}
	}
}

void CMario::OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{
}

void CMario::CrouchProcess(CKeyboardManager* keyboard)
{
	// Khi chuyển qua Crouch, chiều cao của Crouch nhỏ lại => Ta phải thay dổi lại boxsize và localposition

	// Một số ràng buộc khi crouch:
	// Khi vx = 0 và nhấn xuống => Crouch. Khi đang nhấn xuống mà vx != 0 (Đi / chạy qua lại) là chuyển qua ani Đi / chạy
	// Còn đang nhảy (vy != 0) và ấn xuống thì vẫn crouch. Còn 1 lúc bắt k kịp trạng thái bấm qua lại khi đang nhảy (vy != 0 && vx != 0) thì nó có thể vẫn crouch. 

	bool changeAniState = false;
	if (keyboard->GetKeyStateDown(DIK_LEFT) || keyboard->GetKeyStateDown(DIK_RIGHT)) // thiếu xét trường hợp nhảy ******
	{
		// KHÔNG HỤP
		changeAniState = true;
	}
	if ((changeAniState == false && keyboard->GetKeyStateDown(DIK_DOWN)))
	{
		// HỤP
		collisionBoxs->at(0)->SetSizeBox(SUPER_MARIO_CROUCH_BBOX);
		float transformY = SUPER_MARIO_BBOX.y - SUPER_MARIO_CROUCH_BBOX.y;
		collisionBoxs->at(0)->SetPosition(D3DXVECTOR2(0.0f, transformY*0.5f));
		currentPhysicsState.move = MoveOnGroundStates::Crouch;
	}
	else
	{
		// KHÔNG HỤP
		collisionBoxs->at(0)->SetSizeBox(SUPER_MARIO_BBOX);
		collisionBoxs->at(0)->SetPosition(D3DXVECTOR2(0.0f, 0.0f));
	}
}

void CMario::SkidProcess(D3DXVECTOR2 velocity)
{
	auto keyboard = CKeyboardManager::GetInstance();
	int sign = keyboard->GetKeyStateDown(DIK_RIGHT) ? 1 : -1;
	if (isOnGround == true && velocity.x * sign < 0) 
	{
		isSkid = true;
	}
}

void CMario::HoldProcess()
{
	auto keyboard = CKeyboardManager::GetInstance();
	auto normal = physiscBody->GetNormal();
	if (isHold == true && objectHolding != NULL)
	{
		if (keyboard->GetKeyStateDown(DIK_A) == true) // Vẫn đang ấn giữ phím A
		{
			D3DXVECTOR2 posHoldable = transform.position;
			// Chưa xét trường hợp small mario
			posHoldable.x += (SUPER_MARIO_BBOX.x + objectHolding->GetHoldableCollisionBox().x) * normal.x*0.4f;
			objectHolding->SetHoldablePosition(posHoldable);
			objectHolding->SetHoldableNormal(normal);
		}
		else
		{
			// thả ra => vẫn còn nút A => tưởng bị A

			objectHolding->Release();
			objectHolding = NULL;
			isHold = false;
			isKick = true;
		}
	}
}

void CMario::JumpProcess(float jumpForce, bool bounceAfterJumpOnEnemy)
{
	physiscBody->SetVelocity(D3DXVECTOR2(physiscBody->GetVelocity().x, jumpForce));
	isJump = true;
	isOnGround = false;
	this->bounceAfterJumpOnEnemy = bounceAfterJumpOnEnemy;
	if (bounceAfterJumpOnEnemy == true)
		currentPhysicsState.jump = JumpOnAirStates::Jump;
}

void CMario::KickProcess(bool isKick)
{
	this->isKick = isKick;
}

void CMario::StopBounce(bool stopBounce)
{
	this->stopBounce = stopBounce;
}

bool CMario::StopBounce()
{
	return stopBounce;
}

void CMario::KeyState()
{
	
}

void CMario::OnKeyDown(int KeyCode)
{
	// EVENT
	if ((KeyCode == DIK_S || KeyCode == DIK_X) && isOnGround == true && currentPhysicsState.jump == JumpOnAirStates::Stand)
	{
		// JUMP
		JumpProcess(-MARIO_JUMP_FORCE, false);
		if (KeyCode == DIK_S)
		{
			canHighJump = true;
			canLowJumpContinous = false;
			beforeJumpPosition = transform.position.y;

		}
		if (KeyCode == DIK_X)
		{
			canLowJumpContinous = true;
			if (currentPhysicsState.jump == JumpOnAirStates::Stand
				|| currentPhysicsState.jump == JumpOnAirStates::Jump)
				currentPhysicsState.jump = JumpOnAirStates::LowJump;
		}
		if (currentPhysicsState.jump == JumpOnAirStates::Stand && KeyCode == DIK_S)
			currentPhysicsState.jump = JumpOnAirStates::Jump;
	}
	if ((KeyCode == DIK_Z || KeyCode == DIK_A)&& canAttack == true && isAttack == false && currentPhysicsState.move != MoveOnGroundStates::Attack)
	{
		beginAttackTime = GetTickCount64();
		isAttack = true;
	}
}

void CMario::OnKeyUp(int KeyCode)
{
	if (KeyCode == DIK_S)
	{
		canHighJump = false;
	}
}

void CMario::OnDamaged()
{

}

void CMario::HoldObject(CHoldable* holdableObj)
{
	this->objectHolding = holdableObj;
	isHold = true;
	holdableObj->SetHolder(this);
}

void CMario::SetMarioStateTag(MarioStates tag)
{
	marioStateTag = tag;
}

MarioStates CMario::GettMarioStateTag()
{
	return marioStateTag;
}

bool CMario::CanRun()
{
	return currentPhysicsState.move == MoveOnGroundStates::Run;
}

void CMario::Access()
{
	this->isEnabled = true;
	//this->collisionBoxs->at(0)->GetGameObjectAttach()->Enable(true);
	//DebugOut(L"Access: Collision box enable: %d \n", this->collisionBoxs->at(0)->GetGameObjectAttach()->IsEnabled());

}

void CMario::Process()
{

}

void CMario::Exit()
{
	this->isEnabled = false;
	//this->collisionBoxs->at(0)->GetGameObjectAttach()->Enable(false); // solid box . Nếu solid box bị disable thì sao? thì không update
	//DebugOut(L"Exit: Collision box enable: %d \n", this->collisionBoxs->at(0)->GetGameObjectAttach()->IsEnabled());
}

CMario::~CMario()
{
	CGameObject::~CGameObject();

}
