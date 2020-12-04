#include "RedParaGoomba.h"
#include "AnimationManager.h"
#include "ParaGoombaConst.h"
#include "Game.h"
CRedParaGoomba::CRedParaGoomba()
{
	LoadAnimation();
	Init();
	redGoomba = NULL;
}

void CRedParaGoomba::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(PARA_GOOMBA_IDLE, animationManager->Clone("ani-red-para-goomba-idle"));
	AddAnimation(PARA_GOOMBA_WALK, animationManager->Clone("ani-red-para-goomba-walk"));
	AddAnimation(PARA_GOOMBA_FLY, animationManager->Clone("ani-red-para-goomba-fly"));

}

void CRedParaGoomba::Init()
{
	SetState(PARA_GOOMBA_WALK);
	isEnabled = true;
	tag = GameObjectTags::Enemy;
	enemyTag = EnemyTag::ParaGoomba;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(GOOMBA_SIZEBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Para-Goomba");
	collisionBox->SetPosition(D3DXVECTOR2(0.0f, GOOMBA_SIZEBOX.y / 2 - DISTANCE_BETWEEN_PARA_GOOMBA_PIVOT_AND_GOOMBA_PIVOT));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(RED_PARAGOOMBA_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(RED_PARAGOOMBA_SPEED, 0.0f));
}

void CRedParaGoomba::Render(CCamera* cam, int alpha)
{
	if (jumpState == 5 || jumpState == 2 || jumpState == 3 || jumpState == 4)
		SetState(PARA_GOOMBA_FLY);
	else
		SetState(PARA_GOOMBA_WALK);
	CGameObject::Render(cam, alpha);
}

void CRedParaGoomba::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto normal = physiscBody->GetNormal();
	auto vel = physiscBody->GetVelocity();
	if (isOnGround == false)
		return;
	switch (jumpState)
	{
		case 0:
		{
			// Find Mario
			if (target != NULL)
			{
				auto targetPos = target->GetPosition();
				if (GetPosition().x < targetPos.x)
					normal.x = 1;
				if (GetPosition().x > targetPos.x)
					normal.x = -1;
				physiscBody->SetNormal(normal);
			}
			vel.x = RED_PARAGOOMBA_SPEED * normal.x;
			timer = 0;
			jumpState = 1;
			break;
		}
		case 1:
		{
			// Có đồng hồ đếm ngược
			timer += CGame::GetInstance()->GetDeltaTime() * CGame::GetTimeScale();
			if (timer > RED_PARAGOOMBA_BREAK_TIME)
			{
				jumpState = 2;
				timer = 0;
			}
			break;
		}
		case 2: case 3: case 4:
		{
			// Low Jump
			vel.y = -RED_PARAGOOMBA_LOW_JUMP;
			jumpState++;
			isOnGround = false;
			break;
		}
		case 5:
		{
			// High Jump
			vel.y = -RED_PARAGOOMBA_HIGH_JUMP;
			jumpState++;
			isOnGround = false;
			break;
		}
		case 6:
		{
			// Stop
			jumpState = 0;
			break;

		}
	}
	
	physiscBody->SetVelocity(vel);
	//DebugOut(L"VEL GOOMBA %f, %f \n", vel.x, vel.y);
}

bool CRedParaGoomba::IsOnGround()
{
	return isOnGround;
}


