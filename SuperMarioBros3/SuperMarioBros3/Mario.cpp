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
	DebugOut(L"[INFO] Mario Updating.. \n");
	auto game = CGame::GetInstance();

	//TO-DO: xử lý các transform
	
	//SetRotation(1.0f);

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
	//transform.translatePos.x += velocity.x * dt;


}

void CMario::Render()
{
	D3DXVECTOR2 scale = D3DXVECTOR2(3.0f, 3.0f);
	this->SetScale(scale); 
	DebugOut(L"Scale: %f", scale.x);

	this->SetRotation(80.0f); // Bị lỗi
	DebugOut(L"Rotation: %f", transform.rotationAngle);

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
