#include "Mario.h"

#include "AnimationManager.h"
#include "Game.h"
#include "Ultis.h"
#include "MarioConst.h"
#include "Const.h"
#include "Sprite.h"

#include "CollisionBox.h"
#include <cstdlib>

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
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-big-mario-jump"));

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
			currentPhysicsState.move = MoveOnGroundStates::Run;
			// Do chạy gia tốc thay đổi
			physiscBody->SetAcceleration(MARIO_RUNNING_SPEED / dt); // a = v/t *******************
			// Kèm theo lực kéo
			drag.x = MARIO_RUNNING_DRAG_FORCE;
			physiscBody->SetDragForce(drag);
		}
		else
		{
			// Không chạy -> Đi bộ bình thường: WALK
			currentPhysicsState.move = MoveOnGroundStates::Walk;
			physiscBody->SetAcceleration(MARIO_WALKING_SPEED / dt); // *******************
			// Kèm theo lực kéo
			drag.x = MARIO_WALKING_DRAG_FORCE;
			physiscBody->SetDragForce(drag);
		}
		float constSpeed = (currentPhysicsState.move == MoveOnGroundStates::Run) ? MARIO_RUNNING_SPEED : MARIO_WALKING_SPEED;
		if (keyboard->GetKeyStateDown(DIK_RIGHT))
			normal.x = 1;
		else if (keyboard->GetKeyStateDown(DIK_LEFT))
			normal.x = -1;

		targetVelocity.x = normal.x * constSpeed;

		if (velocity.x < targetVelocity.x )
			velocity.x += physiscBody->GetAcceleration();
		else 
			velocity.x -= physiscBody->GetAcceleration();
		// Do ta chỉ cho phép chạy tới 1 khoảng nhất định rồi dừng lại. 
		// Thì việc dừng lại ta sẽ phụ thuộc vào vận tốc
		// Nếu vận tốc tới 1 mức target thì t cho mario dừng lại

	}
	else
	{
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
	}

#pragma endregion

	physiscBody->SetVelocity(velocity);

#pragma region SetStateAnimation And Scale
	SetScale(D3DXVECTOR2(normal.x, 1.0f));
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
	}
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
	if (KeyCode == DIK_SPACE)
	{
		// JUMP
		physiscBody->SetVelocity(D3DXVECTOR2(physiscBody->GetVelocity().x, -0.53f));
		//this->SetState(MARIO_STATE_JUMP);
	}
}

void CMario::OnKeyUp(int KeyCode)
{
}

CMario::~CMario()
{
}
