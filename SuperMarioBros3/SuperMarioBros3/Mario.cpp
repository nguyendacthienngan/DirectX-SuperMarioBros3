#include "Mario.h"

#include "AnimationManager.h"
#include "Game.h"
#include "Ultis.h"
#include "MarioConst.h"
#include "Const.h"
#include "Sprite.h"


CMario::CMario()
{
	LoadAnimation();
	this->SetScale(D3DXVECTOR2(3.0f, 3.0f));
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

	//TO-DO: xét biên 

	transform.translatePos.x += velocity.x * dt;


	//transform.translatePos.y += velocity.y * dt;
	int boundary = NULL;
	auto animFrame = this->animations[currentState]->GetAnimFrame();
	if (animFrame != NULL)
	{
		LPSprite sprite = animFrame->GetSprite();
		boundary = sprite->GetWidth() / 2;
		DebugOut(L"[INFO] boundary: %d \n", boundary);
	}

	if (boundary != NULL)
	{
		if (transform.translatePos.x > SCREEN_WIDTH - boundary * abs(transform.scale.x)) 
			transform.translatePos.x = SCREEN_WIDTH - boundary * abs(transform.scale.x);
		else if (transform.translatePos.x < boundary * abs(transform.scale.x))
			transform.translatePos.x = boundary * abs(transform.scale.x);
	}
	

	// Xử lý input
	if (game->GetKeyDown(DIK_RIGHT))
	{
		velocity.x = MARIO_WALKING_SPEED;
		normal.x = 1;
		SetScale(D3DXVECTOR2(3.0f, 3.0f));
		this->SetState("Run-Right");
		//SetRotation(30.0f);

	}
	if (game->GetKeyDown(DIK_LEFT))
	{
		normal.x = -1;
		velocity.x = normal.x*MARIO_WALKING_SPEED;
		SetScale(D3DXVECTOR2(-3.0f, 3.0f));
		this->SetState("Run-Left");
	}
	/*if (game->GetKeyDown(DIK_DOWN))
	{
		normal.y = 1;
		velocity.y = normal.y * MARIO_WALKING_SPEED;
		this->SetState("Run-Left");
		SetScale(D3DXVECTOR2(3.0f, 3.0f));

	}
	if (game->GetKeyDown(DIK_UP))
	{
		normal.y = -1;
		velocity.y = normal.y * MARIO_WALKING_SPEED;
		this->SetState("Run-Left");
		SetScale(D3DXVECTOR2(3.0f, 3.0f));
	}*/


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
