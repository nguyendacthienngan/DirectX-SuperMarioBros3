﻿#include "Boomerang.h"
#include "AnimationManager.h"
#include "BoomerangBrotherConst.h"

CBoomerang::CBoomerang()
{
	boomerangBrother = NULL;
	LoadAnimation();
	SetState(BOOMERANG_STATE_SPIN);
	isEnabled = true;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(BOOMERANG_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Boomerang");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(0.0f);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));

	attackState = 0;
}

void CBoomerang::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(BOOMERANG_STATE_SPIN, animationManager->Clone("ani-boomerang-spin"));
}

void CBoomerang::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}

void CBoomerang::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto vel = physiscBody->GetVelocity();
	switch (attackState)
	{
		case 0:
		{
			// Phi lên trên bầu trời vòng cung (Gravity = 0)
			// vy < 0, vx > 0 (cùng hướng target hoặc boomerang brother)
			vel.y = -BOOMERANG_VEL_Y;
			vel.x = BOOMERANG_VEL_X;
			if (transform.position.y <= goalPosition.y) // TO-DO: Thiệt ra tạm để là goal.y chứ thiệt ra là hằng số cố định
				attackState = 1;
			break;
		}
		case 1:
		{
			vel.y = BOOMERANG_VEL_Y;
			// Gặp biên => Quay đầu (vx < 0) (Ngược hướng target hoặc boomerang brother) 
			// Khi quay đầu thì giàm ga chậm tí rồi vy cũng tăng
			if (transform.position.x >= goalPosition.x) // TO-DO: Thiệt ra tạm để là goal.x chứ thiệt ra là hằng số cố định
			{
				vel.x = 0;
				attackState = 2;
			}
			break;
		}
		case 2:
		{
			// Rớt xuống (Gravity != 0)
			// vy > 0, vx > 0
			vel.y = BOOMERANG_VEL_Y;
			vel.x -= BOOMERANG_VEL_X / 50;
			if (transform.position.y >= boomerangBrother->GetPosition().y)
			{
				attackState = 3;
			}
			break;
		}
		case 3:
		{
			// Đến khi tới boomerang brother thì Revoke (hoặc ra khỏi camera thì revoke)
			vel.x = -BOOMERANG_VEL_X;
			vel.y = 0;
			if (transform.position.x <= boomerangBrother->GetPosition().x)
			{
				vel.x = 0;
				vel.y = 0;
				attackState = 0;
				pool->Revoke(this);
			}
			break;
		}
	}
	transform.position.x += vel.x * dt;
	physiscBody->SetVelocity(vel);
}

void CBoomerang::LinkToPool(CObjectPool* pool)
{
	this->pool = pool;
}

void CBoomerang::SetGoalPosition(D3DXVECTOR2 goal)
{
	goalPosition = goal;
}

void CBoomerang::SetStartPosition(D3DXVECTOR2 start)
{
	startPosition = start;
}

void CBoomerang::SetBoomerangBrother(CGameObject* gO)
{
	boomerangBrother = gO;
}

bool CBoomerang::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (gO->MarioTag(tag))
		return true;
	return false;
}

void CBoomerang::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (MarioTag(otherCollisionBox->GetGameObjectAttach()->GetTag()))
	{
		auto mario = otherCollisionBox->GetGameObjectAttach();
		//mario->OnDamaged();
	}
}
