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
	this->SetScale(D3DXVECTOR2(3, 3));
	this->SetRotation(0.0f);
}

void CMario::Init()
{
	CGameObject::Init();
	//LoadAnimation(); // chỗ này bị lỗi => Nó k tự gọi Init của nó trong class cha đc !!! Do Init đã được định nghĩa
}

void CMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_IDLE,animationManager->Clone("ani-small-mario-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Clone("ani-small-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Clone("ani-small-mario-run"));

	this->SetState(MARIO_STATE_IDLE); // Để tên đồng nhất với animation
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
	

}

void CMario::Render()
{
	CGameObject::Render();
	
}

void CMario::KeyState(BYTE* states)
{

}

void CMario::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
		case DIK_RIGHT:
		{
			DebugOut(L"[INFO] KEYCODE: RIGHT \n");
			velocity.x = MARIO_WALKING_SPEED;
			normal.x = 1;
			SetScale(D3DXVECTOR2(3, 3));
			this->SetState(MARIO_STATE_WALKING);
			//SetRotation(30.0f);
			break;
		}
		case DIK_LEFT:
		{
			DebugOut(L"[INFO] KEYCODE: LEFT \n");
			normal.x = -1;
			velocity.x = normal.x * MARIO_WALKING_SPEED;
			SetScale(D3DXVECTOR2(-3, 3));
			this->SetState(MARIO_STATE_WALKING);
		}
	}
}

void CMario::OnKeyUp(int KeyCode)
{
}

CMario::~CMario()
{
}
