﻿#include "Mario.h"

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
	LoadAnimation();
	InitProperties();
}

void CMario::Init()
{
	this->SetState(MARIO_STATE_IDLE); // Để tên đồng nhất với animation

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(SUPER_MARIO_BBOX); // Big
	//collisionBox->SetSizeBox(D3DXVECTOR2(14 * 3, 27 * 3)); // Big
	collisionBox->SetPosition(D3DXVECTOR2(0.0f, 0.0f)); // Local Position

	//collisionBox->SetSizeBox(D3DXVECTOR2(12 * 3, 15 * 3)); // Small
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Mario");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);
	

}

void CMario::InitProperties()
{
	currentPhysicsState =
	{
		MoveOnGroundStates::Idle,
		JumpOnAirStates::Stand
	};
	targetVelocity.x = 0.0f;
	targetVelocity.y = 0.0f;

	this->SetScale(D3DXVECTOR2(1.0f, 1.0f));
	tag = GameObjectTags::Player;
	isEnabled = true;
	isOnGround = false;
	this->physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	this->physiscBody->SetDynamic(true); // có chuyển động
	this->physiscBody->SetGravity(MARIO_GRAVITY);
	canHighJump = false;
	isSkid = false;
	previousNormal = physiscBody->GetNormal();
}

void CMario::LoadAnimation()
{
	/*auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-big-mario-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-big-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-big-mario-run"));
	AddAnimation(MARIO_STATE_HIGH_SPEED, animationManager->Get("ani-big-mario-high-speed"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-big-mario-jump"));
	AddAnimation(MARIO_STATE_CROUCH, animationManager->Get("ani-big-mario-crouch"));
	AddAnimation(MARIO_STATE_SKID, animationManager->Get("ani-big-mario-skid"));
	AddAnimation(MARIO_STATE_FALL, animationManager->Get("ani-big-mario-fall"));*/

	/*auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-small-mario-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-small-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-small-mario-run"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-small-mario-jump"));*/

}

void CMario::Update(DWORD dt, CCamera* cam)
{
	CGameObject::Update(dt, cam);
	auto keyboard = CKeyboardManager::GetInstance();
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	previousPhysicsState = currentPhysicsState;
	previousVelocity = velocity;
	previousTargetVelocity = targetVelocity;
	previousNormal = physiscBody->GetNormal();
	physiscBody->SetDragForce(D3DXVECTOR2(MARIO_WALKING_DRAG_FORCE, 0.0f));
	D3DXVECTOR2 drag = physiscBody->GetDragForce();

#pragma region KeyState

	// Horizontal Movement: Walk, Run, Idle
	if (keyboard->GetKeyStateDown(DIK_RIGHT) || keyboard->GetKeyStateDown(DIK_LEFT)) // Có thể nhấn a trước khi nhấn qua lại
	{
		// Nhấn nút A chạy ! RUN
		if (keyboard->GetKeyStateDown(DIK_A))
		{
#pragma region STATE RUN
			currentPhysicsState.move = MoveOnGroundStates::Run;
			// Do chạy gia tốc thay đổi
			if (isSkid == false)
				physiscBody->SetAcceleration(MARIO_RUNNING_ACCELERATION); // a set sẵn
			else
				physiscBody->SetAcceleration(MARIO_SKID_FORCE);
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
			physiscBody->SetAcceleration(MARIO_WALKING_ACCELERATION);
			// Kèm theo lực kéo
			drag.x = MARIO_WALKING_DRAG_FORCE;
			physiscBody->SetDragForce(drag);
#pragma endregion
		}
		float constSpeed;
		if (currentPhysicsState.move == MoveOnGroundStates::Run)
			constSpeed = MARIO_RUNNING_SPEED;
		else if (currentPhysicsState.move == MoveOnGroundStates::Walk)
			constSpeed = MARIO_WALKING_SPEED;
		if (keyboard->GetKeyStateDown(DIK_RIGHT))
		{
			normal.x = 1;
			physiscBody->SetNormal(normal);
			targetVelocity.x = 1 * constSpeed;
		}
		else if (keyboard->GetKeyStateDown(DIK_LEFT))
		{
			normal.x = -1;
			physiscBody->SetNormal(normal);
			targetVelocity.x = -1 * constSpeed;
		}
		// Do ta chỉ cho phép chạy tới 1 khoảng nhất định rồi dừng lại. 
		// Thì việc dừng lại ta sẽ phụ thuộc vào vận tốc
		// Nếu vận tốc tới 1 mức target thì t cho mario dừng lại


#pragma region Speed
		// Tính vận tốc
		// Tại sao phải xét với gia tốc: Để tránh sai số (khi mà đã gần đạt tới mức mà thấp hơn cả gia tốc) thì mình cho nó bằng lun

		if ((abs(velocity.x - targetVelocity.x) <= physiscBody->GetAcceleration() * dt))
			velocity.x = targetVelocity.x;
		else
		{
			if (velocity.x < targetVelocity.x)
				velocity.x += physiscBody->GetAcceleration()*dt;
			else
				velocity.x -= physiscBody->GetAcceleration()*dt;
		}

#pragma endregion
		physiscBody->SetNormal(normal);
		physiscBody->SetVelocity(velocity);

		SkidProcess(velocity);
		if (isSkid == true)
			currentPhysicsState.move = MoveOnGroundStates::Skid;
	}
	else
	{
#pragma region STATE IDLE
		// Dừng mario
		// lực kéo sẽ giảm vận tốc lại để cho vận tốc mario đi về 0
		// khi vx < lực kéo rồi thì set vx = 0 luôn r cho nó đứng lại để tránh sai số
		if (abs(velocity.x) <= physiscBody->GetDragForce().x)
		{
			velocity.x = 0;
			if (currentPhysicsState.move != MoveOnGroundStates::Idle)
				currentPhysicsState.move = MoveOnGroundStates::Idle;
		}
		else
			velocity.x = (abs(velocity.x) - physiscBody->GetDragForce().x);
		velocity.x *= normal.x;
		isSkid = false;
		physiscBody->SetNormal(normal);
		physiscBody->SetVelocity(velocity);
		
#pragma endregion
	}

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
	if (currentPhysicsState.jump == JumpOnAirStates::Jump && canLowJumpContinous == false)
	{
		auto jumpForce = MARIO_JUMP_FORCE;
		isHighJump = false;

		// Nhảy liên tục: Chỉ cần cung cấp dy < 0 và có gravity thì ta tạo cảm giác nó nhảy liên tục chuyển động đều
		if (keyboard->GetKeyStateDown(DIK_S) && canHighJump == true &&
			(velocity.y >= -MARIO_HIGH_JUMP_FORCE && velocity.y <= MARIO_JUMP_FORCE * (-0.5)))
		{
			jumpForce = MARIO_HIGH_JUMP_FORCE;
			isHighJump = true;
		}
		if (velocity.y > -jumpForce && velocity.y < 0 && isHighJump == true) //  isHighJump hay canJump : đang hướng về target
		{
			physiscBody->SetGravity(0.0f);
			velocity.y -= MARIO_PUSH_FORCE;
		}
		else
		{
			// Đạt max high jump
			velocity.y = -1 * jumpForce; // tránh sai số
			physiscBody->SetGravity(MARIO_GRAVITY);
			currentPhysicsState.jump = JumpOnAirStates::HighJump;
		}
	}
	else if (currentPhysicsState.jump == JumpOnAirStates::HighJump)
	{
		if (velocity.y > 0) // Hướng xuống 
		{
			canHighJump = false;
			currentPhysicsState.jump = JumpOnAirStates::Fall;
		}
	}
	else if (currentPhysicsState.jump == JumpOnAirStates::Fall)
	{
		if (isOnGround == true)
			currentPhysicsState.jump = JumpOnAirStates::Stand;
	}


#pragma endregion

	physiscBody->SetVelocity(velocity);
	physiscBody->SetNormal(normal);

	SetScale(D3DXVECTOR2(normal.x, 1.0f));

	if (canCrouch == true) // Small Mario k thể crouch
		CrouchProcess(keyboard);
	
}

void CMario::Render(CCamera* cam)
{
#pragma region Update State

#pragma region Move On Ground
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
	case MoveOnGroundStates::Skid:
	{
		auto normal = physiscBody->GetNormal();
		SetScale(D3DXVECTOR2(-normal.x, 1.0f));
		SetState(MARIO_STATE_SKID);
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
	case MoveOnGroundStates::Spin:
	{
		SetState(MARIO_STATE_SPIN);
		break;
	}
	}
#	pragma endregion

#pragma region Jump On Air

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
	}
#pragma endregion

#pragma endregion

#pragma region Multipiler
	auto animation = GetAnimationByState(currentState);
	auto speed = abs(physiscBody->GetVelocity().x);
	auto multiplier = 1.0f;
	if (animation != NULL)
	{
		if (currentState == MARIO_STATE_RUNNING || currentState == MARIO_STATE_WALKING)
		{
			multiplier = speed / MARIO_WALKING_SPEED;
			animation->SetSpeedMultiplier(Clamp(multiplier, 1.0f, 3.0f)); // Bị lỗi đối với small mario khi small mario chưa có high speed;
			DebugOut(L"Speed/walkSpeed: %f \n", speed / MARIO_WALKING_SPEED);
			DebugOut(L"Multiplier: %f \n", multiplier);
		}
		else
			animation->ResetSpeedMultiplier();
	}
	
#pragma endregion

	SetRelativePositionOnScreen(collisionBoxs->at(0)->GetPosition());
	CGameObject::Render(cam);
}

void CMario::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Solid)
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
	if ((changeAniState == false && keyboard->GetKeyStateDown(DIK_DOWN))
		|| currentPhysicsState.jump == JumpOnAirStates::Jump)
	{
		// HỤP
		collisionBoxs->at(0)->SetSizeBox(SUPER_MARIO_CROUCH_BBOX);
		float transformY = SUPER_MARIO_BBOX.y - SUPER_MARIO_CROUCH_BBOX.y;
		collisionBoxs->at(0)->SetPosition(D3DXVECTOR2(0.0f, transformY));
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
	// Ta detect skid (thắng lại) theo tốc độ của mario chứ không theo state, 
	//bởi vì có thể state đã chuyển (VD: từ Run sang Walk) nhưng vẫn cần thời gian gia tốc xuống, giữa khoảng gia tốc xuống đó mà không loại trường hợp đó ra
	// Sẽ detect nhầm qua skid
	// Do đó ta phải đảm bảo tốc độ nằm ngoài khoảng nhầm lẫn đó thì skid mới xảy ra
	
	bool skidMovementConstraint =
		(currentPhysicsState.move == MoveOnGroundStates::Walk &&
			(abs(velocity.x) < MARIO_WALKING_SPEED && abs(velocity.x) > MARIO_RUNNING_SPEED))
		|| (currentPhysicsState.move == MoveOnGroundStates::Run);

	auto normal = physiscBody->GetNormal();
	bool skidFacingConstraint = ((previousVelocity.x < velocity.x&& normal.x == -1
		|| previousVelocity.x > velocity.x && normal.x == 1
		));

	if (isSkid == false && skidFacingConstraint == true && skidMovementConstraint == true)
	{
		isSkid = true;
		currentPhysicsState.move = MoveOnGroundStates::Skid;
	}
	else if (Sign(velocity.x) != Sign(previousVelocity.x) || normal.x != previousNormal.x)
		isSkid = false; // Sau khi lật xong phía bên kia thì dừng skid !!!!!!!
}

void CMario::KeyState()
{

}

void CMario::OnKeyDown(int KeyCode)
{
	// EVENT
	if ((KeyCode == DIK_S || KeyCode == DIK_X) && isOnGround == true)
	{
		// JUMP
		physiscBody->SetVelocity(D3DXVECTOR2(physiscBody->GetVelocity().x, -MARIO_JUMP_SPEED_Y)); // Vẫn còn bị trọng lực kéo mạnh quá
		isOnGround = false;
		if (KeyCode == DIK_S)
		{
			canHighJump = true;
			canLowJumpContinous = false;
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
	if (KeyCode == DIK_Z)
	{
		currentPhysicsState.move = MoveOnGroundStates::Spin;
	}
}

void CMario::OnKeyUp(int KeyCode)
{
	if (KeyCode == DIK_S)
	{
		canHighJump = false;
	}
	if (KeyCode == DIK_RIGHT)
	{
		currentPhysicsState.move = MoveOnGroundStates::Idle;
		
	}
	if (KeyCode == DIK_LEFT)
	{
		currentPhysicsState.move = MoveOnGroundStates::Idle;

	}
	/*if (KeyCode == DIK_Z || currentPhysicsState.move == MoveOnGroundStates::Spin)
	{
		currentPhysicsState.move = MoveOnGroundStates::Idle;

	}*/
}

void CMario::Access()
{
	this->isEnabled = true;
	//this->collisionBoxs->at(0)->GetGameObjectAttach()->Enable(true);
	//DebugOut(L"Access: Collision box enable: %d \n", this->collisionBoxs->at(0)->GetGameObjectAttach()->IsEnabled());

}

void CMario::Update()
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
}
