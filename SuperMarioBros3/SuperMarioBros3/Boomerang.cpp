#include "Boomerang.h"
#include "AnimationManager.h"
#include "BoomerangBrotherConst.h"

CBoomerang::CBoomerang()
{
	tag = GameObjectTags::Boomerang;
	boomerangBrother = NULL;
	LoadAnimation();
	SetState(BOOMERANG_STATE_IDLE);
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

	attackState = -1;
}

void CBoomerang::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(BOOMERANG_STATE_SPIN, animationManager->Clone("ani-boomerang-spin"));
	AddAnimation(BOOMERANG_STATE_IDLE, animationManager->Clone("ani-boomerang-idle"));
}

void CBoomerang::Render(CCamera* cam, int alpha)
{
	if (attackState >= 0)
		SetState(BOOMERANG_STATE_SPIN);
	else
		SetState(BOOMERANG_STATE_IDLE);
	CGameObject::Render(cam, alpha);
}

void CBoomerang::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto vel = physiscBody->GetVelocity();
	auto normal = boomerangBrother->GetPhysiscBody()->GetNormal();
	physiscBody->SetNormal(normal);
	switch (attackState)
	{
		case -1:
		{
			transform.position = boomerangBrother->GetPosition();
			transform.position.x -= BOOMERANG_BROTHER__BBOX.x * 0.5f;
			transform.position.y -= BOOMERANG_BROTHER__BBOX.y * 0.5f;
			startPosition = transform.position;
		}
		case 0:
		{

			// Phi lên trên bầu trời vòng cung (Gravity = 0)
			// vy < 0, vx > 0 (cùng hướng target hoặc boomerang brother)
			if (normal.x > 0)
			{
				if (transform.position.x < BOOMERANG_LIMIT_RIGHT)
					vel.x = BOOMERANG_VEL_X;
				else
					vel.x = 0;
			}
			else
			{
				if (transform.position.x > -BOOMERANG_LIMIT_RIGHT)
					vel.x = -BOOMERANG_VEL_X;
				else
					vel.x = 0;
			}
			if (transform.position.y <= BOOMERANG_LIMIT_TOP)
			{
				attackState = 1;
				vel.y = BOOMERANG_VEL_Y;
			}
			else 
				vel.y = -BOOMERANG_VEL_Y;
			break;
		}
		case 1:
		{
			// Gặp biên => Quay đầu (vx < 0) (Ngược hướng target hoặc boomerang brother) 
			// Khi quay đầu thì giàm ga chậm tí rồi vy cũng tăng
			if (normal.x > 0)
			{
				if (transform.position.x >= BOOMERANG_LIMIT_RIGHT)
				{
					vel.x = 0;
					attackState = 2;
				}
				else
					vel.y = BOOMERANG_VEL_Y;

			}
			else
			{
				if (transform.position.x >= BOOMERANG_LIMIT_RIGHT)
				{
					vel.x = 0;
					attackState = 2;
				}
				else
					vel.y = BOOMERANG_VEL_Y;
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
				attackState = -1;
				transform.position = boomerangBrother->GetPosition();
				transform.position.x -= BOOMERANG_BROTHER__BBOX.x * 0.5f;
				transform.position.y -= BOOMERANG_BROTHER__BBOX.y * 0.5f;
				startPosition = transform.position;
				pool->Revoke(this);
			}
			break;
		}
	}
	if (attackState != -1)
	{
		transform.position.x += vel.x * dt;

	}
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
		mario->OnDamaged();
	}
}

void CBoomerang::SetAttackState(int attackState)
{
	this->attackState = attackState;
}

int CBoomerang::GetAttackState()
{
	return attackState;
}
