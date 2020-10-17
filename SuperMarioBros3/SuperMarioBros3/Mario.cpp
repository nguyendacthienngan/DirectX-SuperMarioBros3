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
	this->SetScale(D3DXVECTOR2(1.0f, 1.0f));
	tag = "player";
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

void CMario::Update(DWORD dt, CCamera* cam, std::vector<LPGameObject>* coObjects)
{
	CGameObject::Update(dt, cam, coObjects);
	DebugOut(L"[INFO] Mario Updating.. \n");
	auto game = CGame::GetInstance();

	// Lúc này là milisecond
	dt = CGame::GetInstance()->GetFixedDeltaTime();  // Không bị viền
	transform.position.x += velocity.x * CGame::GetInstance()->GetFixedDeltaTime();
	//transform.translatePos.x = distance.x;

	int bound = NULL; // biên của Sprite
	auto animFrame = this->animations[currentState]->GetAnimFrame();
	if (animFrame != NULL)
	{
		LPSprite sprite = animFrame->GetSprite();
		bound = sprite->GetWidth() / 2; // 16 /2 = 8
	}


	D3DXVECTOR2 distanceInCam = cam->Transform(transform.position);
	if (bound != NULL)
	{
		// Xét biên lớn 0 - 800 theo hệ quy chiếu camera để mario k đi ra khỏi camera
		if (distanceInCam.x > SCREEN_WIDTH - bound * abs(transform.scale.x))
			distanceInCam.x = SCREEN_WIDTH - bound * abs(transform.scale.x);
		else if (distanceInCam.x < bound * abs(transform.scale.x))
			distanceInCam.x = bound * abs(transform.scale.x);
	}
	transform.position = cam->TransformCamToWorld(distanceInCam);

	cam->SetPositionMario(distanceInCam); // Tọa độ theo camera
	cam->SetSpeedMario(velocity); // Vận tốc theo camera
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
			this->SetState(MARIO_STATE_WALKING);
			SetScale(D3DXVECTOR2(1.0f, 1.0f));

			//SetRotation(30.0f);
			break;
		}
		case DIK_LEFT:
		{
			DebugOut(L"[INFO] KEYCODE: LEFT \n");
			normal.x = -1;
			velocity.x = normal.x * MARIO_WALKING_SPEED;
			SetScale(D3DXVECTOR2(-1.0f, 1.0f));
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
