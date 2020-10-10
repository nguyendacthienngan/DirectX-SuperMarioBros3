#include "Mario.h"

#include "AnimationManager.h"
#include "Game.h"
#include "Ultis.h"
#include "MarioConst.h"
CMario::CMario()
{
	LoadAnimation();

}

void CMario::Init()
{
	//LoadAnimation(); // chỗ này bị lỗi => Nó k tự gọi Init của nó trong class cha đc !!! Do Init đã được định nghĩa
}

void CMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("Idle",animationManager->Clone("ani-small-mario-idle"));
	AddAnimation("Run",animationManager->Clone("ani-small-mario-run"));

	this->SetState("Idle"); // Để tên đồng nhất với animation
}

void CMario::Update(DWORD dt, std::vector<LPGameObject>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	//TO-DO: xử lý các transform
	DebugOut(L"[INFO] Mario Updating.. \n");
	auto game = CGame::GetInstance();

	// Xử lý input
	if (game->GetKeyDown(DIK_RIGHT))
	{
		velocity.x = MARIO_WALKING_SPEED;
		normal.x = 1;
		this->SetState("Run");
	}
	if (game->GetKeyDown(DIK_LEFT))
	{
		velocity.x = 0;
		normal.x = 1;
		this->SetState("Idle");
	}
	position.x += velocity.x * dt;


}

void CMario::OnKeyDown(int KeyCode)
{
}

void CMario::OnKeyUp(int KeyCode)
{
}

CMario::~CMario()
{
}
