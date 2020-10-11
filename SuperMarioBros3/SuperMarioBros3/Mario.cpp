#include "Mario.h"

#include "AnimationManager.h"
#include "Game.h"
#include "Ultis.h"
#include "MarioConst.h"
CMario::CMario()
{
	LoadAnimation();
	this->SetScale(D3DXVECTOR2(0.0f, 0.0f));
	this->SetRotation(0.0f);

}

void CMario::Init()
{
	//LoadAnimation(); // chỗ này bị lỗi => Nó k tự gọi Init của nó trong class cha đc !!! Do Init đã được định nghĩa
}

void CMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("Idle",animationManager->Clone("ani-small-mario-idle"));
	AddAnimation("Run-Right", animationManager->Clone("ani-small-mario-run"));
	AddAnimation("Run-Left",animationManager->Clone("ani-small-mario-run"));

	this->SetState("Idle"); // Để tên đồng nhất với animation
}

void CMario::Update(DWORD dt, std::vector<LPGameObject>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	DebugOut(L"[INFO] Mario Updating.. \n");
	auto game = CGame::GetInstance();

	//TO-DO: xử lý các transform
	
	//SetRotation(1.0f);

	// Xử lý input
	if (game->GetKeyDown(DIK_RIGHT))
	{
		velocity.x = MARIO_WALKING_SPEED;
		normal.x = 1;
		this->SetState("Run-Right");
	}
	if (game->GetKeyDown(DIK_LEFT))
	{
		normal.x = -1;
		velocity.x = normal.x*MARIO_WALKING_SPEED;
		this->SetState("Run-Left");
	}
	transform.translatePos.x += velocity.x * dt;


}

void CMario::Render()
{
	CGameObject::Render();

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
