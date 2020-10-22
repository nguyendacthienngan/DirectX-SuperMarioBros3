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
	tag = GameObjectTags::Player;
	isEnabled = true;
	isOnGround = false; // ?
	this->physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	this->physiscBody->SetDynamic(true); // có chuyển động
	this->physiscBody->SetGravity(MARIO_GRAVITY);
}

void CMario::Init()
{
	this->SetState(MARIO_STATE_IDLE); // Để tên đồng nhất với animation

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(BIG_MARIO_BBOX); // Big
	//collisionBox->SetSizeBox(BIG_MARIO_BBOX); // Big
	//collisionBox->SetSizeBox(D3DXVECTOR2(14 * 3, 27 * 3)); // Big
	collisionBox->SetPosition(D3DXVECTOR2(0.0f, 0.0f)); // ??? Local Position?

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

	/*AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-small-mario-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-small-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-small-mario-run"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-small-mario-jump"));*/
}


void CMario::Update(DWORD dt, CCamera* cam)
{
	//CGameObject::Update(dt, cam);
	DebugOut(L"[INFO] Mario Updating.. \n");
	auto keyboard = CKeyboardManager::GetInstance();
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	previousPhysicsState = currentPhysicsState;
	previousVelocity = velocity;
	previousTargetVelocity = targetVelocity;
	physiscBody->SetDragForce(D3DXVECTOR2(MARIO_WALKING_DRAG_FORCE, 0.0f));
	D3DXVECTOR2 drag = physiscBody->GetDragForce();
#pragma region KeyState
	
	// Horizontal Movement: Walk, Run, Idle
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

		#pragma region Speed
		// Tính vận tốc
		if (abs(velocity.x - targetVelocity.x) > physiscBody->GetAcceleration()) 
			// Tại sao phải xét với gia tốc: Để tránh sai số (khi mà đã gần đạt tới mức mà thấp hơn cả gia tốc) thì mình cho nó bằng lun
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
				velocity.x -= abs(physiscBody->GetAcceleration());
			}
		}
		else
		{
			velocity.x = physiscBody->GetAcceleration();
			//velocity.x = targetVelocity.x;
		}
		isHighSpeed = (velocity.x > MARIO_RUNNING_SPEED * 0.9); 
		if (isHighSpeed == true && currentPhysicsState.move == MoveOnGroundStates::Run)
			currentPhysicsState.move = MoveOnGroundStates::HighSpeed;
		/*DebugOut(L"Current speed %f \n", velocity.x);
		DebugOut(L"Target speed: %f \n", targetVelocity.x);*/
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
		DebugOut(L"Da nhan X \n");
		velocity.y -= MARIO_JUMP_SPEED_Y;
		isOnGround = false;
		currentPhysicsState.jump = JumpOnAirStates::LowJumpContinuous;
	}
	if (currentPhysicsState.jump == JumpOnAirStates::Jump)
	{
		auto jumpForce = MARIO_JUMP_FORCE;
		isHighJump = false;
		// Nhảy liên tục: Chỉ cần cung cấp dy < 0 và có gravity thì ta tạo cảm giác nó nhảy liên tục chuyển động đều
		
		if (keyboard->GetKeyStateDown(DIK_S) && canHighJump == true && (velocity.y >= -MARIO_HIGH_JUMP_FORCE && velocity.y <= MARIO_JUMP_FORCE * (-0.5)))
		{
			jumpForce = MARIO_HIGH_JUMP_FORCE;
			isHighJump = true;
		}
		if (velocity.y > -jumpForce && velocity.y < 0 && isHighJump == true) //  isHighJump hay canJump
		{
			// đang high jump (hướng đi lên) 
			physiscBody->SetGravity(0.0f);
			velocity.y -= MARIO_PUSH_FORCE; 
		}
		else
		{
			// Hỏng hiểu
			velocity.y = -1 * jumpForce;
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
	
	CrouchProcess(keyboard);

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
		animation->SetSpeedMultiplier(Clamp(multiplier, 1.0f, 3.0f)); // Bị lỗi đối với small mario khi small mario chưa có high speed;
		DebugOut(L"Speed/walkSpeed: %f \n", speed / MARIO_WALKING_SPEED);
		DebugOut(L"Multiplier: %f \n", multiplier);
	}	
	else
		animation->ResetSpeedMultiplier();
#pragma endregion

	SetRelativePositionOnScreen(collisionBoxs->at(0)->GetPosition());
}

void CMario::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) // ??
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Solid)
		{
			// ny < 0 (hướng lên trên), nx < 0.000001f ?
			if (collisionEvent->ny < 0 && isOnGround == false)
			{
				isOnGround = true;
				DebugOut(L"OnGround\n");

			}
			if (collisionEvent->nx != 0) // vừa ấn nhảy vừa ấn qua trái phải
			{
				auto v = physiscBody->GetVelocity();
				physiscBody->SetVelocity(D3DXVECTOR2(0, v.y)); // ?
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

	if (changeAniState == false && keyboard->GetKeyStateDown(DIK_DOWN))
	{
		// HỤP
		collisionBoxs->at(0)->SetSizeBox(BIG_MARIO_CROUCH_BBOX);
		float transformY = BIG_MARIO_BBOX.y - BIG_MARIO_CROUCH_BBOX.y;
		//transform = -transform;
		//transform.x *= 0.5f;
		//transform *= 0.5f;
		collisionBoxs->at(0)->SetPosition(D3DXVECTOR2(0.0f, transformY));
		
		currentPhysicsState.move = MoveOnGroundStates::Crouch;
	}
	else
	{
		// KHÔNG HỤP
		collisionBoxs->at(0)->SetSizeBox(BIG_MARIO_BBOX);

		collisionBoxs->at(0)->SetPosition(D3DXVECTOR2(0.0f, 0.0f));

	}
}

void CMario::SkidProcess()
{
}

void CMario::KeyState()
{
	

}

void CMario::OnKeyDown(int KeyCode)
{
	// EVENT
	if ( (KeyCode == DIK_S || KeyCode == DIK_X) && isOnGround == true)
	{
		// JUMP
		physiscBody->SetVelocity(D3DXVECTOR2(physiscBody->GetVelocity().x, -MARIO_JUMP_SPEED_Y)); // Vẫn còn bị trọng lực kéo mạnh quá
		isOnGround = false;
		canHighJump = true;
		if (currentPhysicsState.jump == JumpOnAirStates::Stand)
			currentPhysicsState.jump = JumpOnAirStates::Jump;
	}
	
}

void CMario::OnKeyUp(int KeyCode)
{
	if (KeyCode == DIK_S)
	{
		canHighJump = false; // thả nút ra là cho nó ngừng nhảy cao lièn
	}
}

CMario::~CMario()
{
}
