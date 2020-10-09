#include "Mario.h"

#include "AnimationManager.h"
#include "Game.h"
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
	AddAnimation("Run",animationManager->Clone("ani-small-mario-run"));

	this->SetState("Run"); // Để tên đồng nhất với animation
}

void CMario::Update()
{
	//TO-DO: xử lý các transform

	// Xử lý input
	auto game = CGame::GetInstance();
	/*if (game->GetKeyDown(DIK_LEFT))
	{
		
	}
	else if (game->GetKeyDown(DIK_RIGHT))
	{

	}*/
	
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
