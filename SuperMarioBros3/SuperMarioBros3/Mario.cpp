#include "Mario.h"

#include "AnimationManager.h"
#include "Game.h"
#include "Ultis.h"
#include "MarioConst.h"
#include "Const.h"
#include "Sprite.h"

#include "CollisionBox.h"
#include <cstdlib>
#include <cctype> 
#include <string>

using namespace std;

CMario::CMario()
{
	Init();
	LoadAnimation();

	this->SetScale(D3DXVECTOR2(1.0f, 1.0f));
	tag = "player";
	isEnabled = true;
	normal.x = 1;
	normal.y = 1;

	this->physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	this->physiscBody->SetDynamic(true); // có chuyển động
	this->physiscBody->SetGravity(MARIO_GRAVITY);
}

void CMario::Init()
{
	this->SetState(MARIO_STATE_IDLE); // Để tên đồng nhất với animation

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(D3DXVECTOR2(14 * 3, 27 * 3)); // Big
	//collisionBox->SetSizeBox(D3DXVECTOR2(12 * 3, 15 * 3)); // Small
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Mario");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	currentPhysicsState = 
	{ 
		MoveOnGroundStates::Idle,
		JumpOnAirStates::Stand
	};
	targetVelocity.x = 0.0f;
	targetVelocity.y = 0.0f;
}

void CMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-big-mario-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-big-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-big-mario-run"));
	AddAnimation(MARIO_STATE_HIGH_SPEED, animationManager->Get("ani-big-mario-high-speed"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-big-mario-jump"));
	AddAnimation(MARIO_STATE_CROUCH, animationManager->Get("ani-big-mario-crouch")); // * BỊ LỖI VÌ KHI ĐỔI BOUNDINGBOX cái map bị lệch. Chưa biết nguyên nhân, có thể do khi draw map theo camera bị lệch

	//AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-small-mario-idle"));
	//AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-small-mario-walk"));
	//AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-small-mario-run"));
	//AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-small-mario-jump"));
}


void CMario::Update(DWORD dt, CCamera* cam)
{
	CGameObject::Update(dt, cam);
	//DebugOut(L"[INFO] Mario Updating.. \n");
	auto keyboard = CKeyboardManager::GetInstance();
	auto velocity = physiscBody->GetVelocity();
	//float acceleration = physiscBody->GetAcceleration();
	//auto speed = velocity.x;
	previousPhysicsState = currentPhysicsState;
	previousVelocity = velocity;
	previousTargetVelocity = targetVelocity;
	physiscBody->SetDragForce(D3DXVECTOR2(MARIO_WALKING_DRAG_FORCE, 0.0f));
	D3DXVECTOR2 drag = physiscBody->GetDragForce();
#pragma region KeyState

	if (keyboard->GetKeyStateDown(DIK_RIGHT) || keyboard->GetKeyStateDown(DIK_LEFT))
	{

		// Nhấn nút A chạy ! RUN
		if (keyboard->GetKeyStateDown(DIK_A))
		{
			#pragma region STATE RUN
			currentPhysicsState.move = MoveOnGroundStates::Run;
			// Do chạy gia tốc thay đổi
			physiscBody->SetAcceleration( (MARIO_RUNNING_SPEED - abs(velocity.x)) / dt); // a = v/t *******************
			//physiscBody->SetAcceleration(MARIO_RUNNING_ACCELERATION); // a set sẵn
			// Kèm theo lực kéo
			drag.x = MARIO_RUNNING_DRAG_FORCE;
			physiscBody->SetDragForce(drag);
			#pragma endregion
		}
		else
		{
			#pragma region STATE WALK
			// Không chạy -> Đi bộ bình thường: WALK
			currentPhysicsState.move = MoveOnGroundStates::Walk;
			physiscBody->SetAcceleration( (MARIO_WALKING_SPEED - abs(velocity.x)) / dt); // *******************
			//physiscBody->SetAcceleration(MARIO_WALKING_ACCELERATION); // *******************
			// Kèm theo lực kéo
			drag.x = MARIO_WALKING_DRAG_FORCE;
			physiscBody->SetDragForce(drag);
			#pragma endregion
		}
		float constSpeed = (currentPhysicsState.move == MoveOnGroundStates::Run) ? MARIO_RUNNING_SPEED : MARIO_WALKING_SPEED;
		if (keyboard->GetKeyStateDown(DIK_RIGHT))
			normal.x = 1;
		else if (keyboard->GetKeyStateDown(DIK_LEFT))
			normal.x = -1;

		// Do ta chỉ cho phép chạy tới 1 khoảng nhất định rồi dừng lại. 
		// Thì việc dừng lại ta sẽ phụ thuộc vào vận tốc
		// Nếu vận tốc tới 1 mức target thì t cho mario dừng lại

		targetVelocity.x = normal.x * constSpeed;

		// Tính vận tốc


		if (abs(velocity.x - targetVelocity.x) > physiscBody->GetAcceleration()) // Tại sao phải xét với gia tốc
		{

			if (velocity.x < targetVelocity.x)
			{
				velocity.x += abs(physiscBody->GetAcceleration());
				/*if (currentPhysicsState.move == MoveOnGroundStates::HighSpeed)
					currentPhysicsState.move = MoveOnGroundStates::Run;*/
			}
			else
			{
				//if (velocity.x == targetVelocity.x)
				DebugOut(L"High Speed \n");
				if (currentPhysicsState.move == MoveOnGroundStates::Run)
				{
					currentPhysicsState.move = MoveOnGroundStates::HighSpeed;

				}
				velocity.x -= abs(physiscBody->GetAcceleration());

			}
		}
		else
		{
			velocity.x = physiscBody->GetAcceleration();
			//velocity.x = targetVelocity.x;
		}
		DebugOut(L"Current speed %f \n", velocity.x);
		DebugOut(L"Target speed: %f \n", targetVelocity.x);

	}
	else if (keyboard->GetKeyStateDown(DIK_DOWN))
	{
		#pragma region STATE CROUCH
		currentPhysicsState.move = MoveOnGroundStates::Crouch;
		#pragma endregion
	}
	else
	{
		#pragma region STATE IDLE
		//DebugOut(L"Stop \n");
		// Dừng mario
		// lực kéo sẽ giảm vận tốc lại để cho vận tốc mario đi về 0
		if (abs(velocity.x) > physiscBody->GetDragForce().x)
			velocity.x = abs(velocity.x) - physiscBody->GetDragForce().x;
		else
		{
			velocity.x = 0;
			if (currentPhysicsState.move != MoveOnGroundStates::Idle)
				currentPhysicsState.move = MoveOnGroundStates::Idle;
		}
		velocity.x *= normal.x;
		#pragma endregion
	}



	physiscBody->SetVelocity(velocity);
	SetScale(D3DXVECTOR2(normal.x, 1.0f));


}

void CMario::LateUpdate()
{
#pragma region Update State
	switch (currentPhysicsState.move)
	{
	case MoveOnGroundStates::Idle:
	{
		SetState(MARIO_STATE_IDLE);

		break;
	}
	case MoveOnGroundStates::Walk:
	{
		SetState(MARIO_STATE_WALKING);
		break;
	}
	case MoveOnGroundStates::Run:
	{
		SetState(MARIO_STATE_RUNNING);
		break;
	}
	case MoveOnGroundStates::Crouch:
	{
		SetState(MARIO_STATE_CROUCH);
		break;
	}
	case MoveOnGroundStates::HighSpeed:
	{
		SetState(MARIO_STATE_HIGH_SPEED);
		break;
	}
	}
#pragma endregion

#pragma region Multipiler
	auto animation = GetAnimationByState(currentState);
	auto speed = abs(physiscBody->GetVelocity().x);
	auto multiplier = 1.0f;
	if (currentState == MARIO_STATE_RUNNING || currentState == MARIO_STATE_WALKING)
	{
		multiplier = speed / MARIO_WALKING_SPEED;
		animation->SetSpeedMultiplier(Clamp(multiplier, 1.0f, 3.0f));
		DebugOut(L"Speed/walkSpeed: %f \n", speed / MARIO_WALKING_SPEED);
		DebugOut(L"Multiplier: %f \n", multiplier);
	}	
	else
		animation->ResetSpeedMultiplier();
#pragma endregion
}

void CMario::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{
}

void CMario::OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{
}

void CMario::KeyState()
{
	

}

void CMario::OnKeyDown(int KeyCode)
{
	// EVENT
	if (KeyCode == DIK_S)
	{
		// HIGH JUMP
		physiscBody->SetVelocity(D3DXVECTOR2(physiscBody->GetVelocity().x, -0.53f));
		//this->SetState(MARIO_STATE_JUMP);
	}
	if (KeyCode == DIK_X)
	{
		physiscBody->SetVelocity(D3DXVECTOR2(physiscBody->GetVelocity().x, -0.4f));
	}
}

void CMario::OnKeyUp(int KeyCode)
{
}

CMario::~CMario()
{
}
