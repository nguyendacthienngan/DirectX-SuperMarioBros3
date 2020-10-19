#include "Mario.h"

#include "AnimationManager.h"
#include "Game.h"
#include "Ultis.h"
#include "MarioConst.h"
#include "Const.h"
#include "Sprite.h"

#include "CollisionBox.h"

CMario::CMario()
{
	Init();
	this->SetScale(D3DXVECTOR2(1.0f, 1.0f));
	tag = "player";
	isEnabled = true;

}

void CMario::Init()
{
	LoadAnimation(); 
	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(D3DXVECTOR2(12 * 3, 15 * 3)); // Hard code
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Mario");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	this->physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	this->physiscBody->SetDynamic(true); // có chuyển động
	this->physiscBody->SetGravity(0.0006f); // hard code
}

void CMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_IDLE,animationManager->Get("ani-small-mario-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-small-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-small-mario-run"));

	this->SetState(MARIO_STATE_IDLE); // Để tên đồng nhất với animation
}

void CMario::Update(DWORD dt, CCamera* cam)
{
	CGameObject::Update(dt, cam);
	//DebugOut(L"[INFO] Mario Updating.. \n");
	float facing = 1.0f;
#pragma region KeyState
	auto keyboard = CKeyboardManager::GetInstance();
	auto velocity = physiscBody->GetVelocity();
	auto speed = velocity.x;

	if (keyboard->GetKeyStateDown(DIK_RIGHT))
	{
		//DebugOut(L"[INFO] KEYCODE: RIGHT \n");
		velocity.x = MARIO_WALKING_SPEED;
		normal.x = 1;
		this->SetState(MARIO_STATE_WALKING);
		SetScale(D3DXVECTOR2(1.0f, 1.0f));
	}
	else if (keyboard->GetKeyStateDown(DIK_LEFT))
	{
		//DebugOut(L"[INFO] KEYCODE: LEFT \n");
		normal.x = -1;
		velocity.x = normal.x * MARIO_WALKING_SPEED;
		SetScale(D3DXVECTOR2(-1.0f, 1.0f));
		this->SetState(MARIO_STATE_WALKING);
	}
	else
	{
		this->SetState(MARIO_STATE_IDLE);
		//facing = (normal.x > 0) ? 1.0f : -1.0f;
		SetScale(D3DXVECTOR2(normal.x, 1.0f));
		velocity.x = 0.0f;
		//velocity.y = 0.0f;

	}
#pragma endregion
	physiscBody->SetVelocity(velocity);
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
		physiscBody->SetVelocity(D3DXVECTOR2(physiscBody->GetVelocity().x, -0.3f));
	}
}

void CMario::OnKeyUp(int KeyCode)
{
}

CMario::~CMario()
{
}
