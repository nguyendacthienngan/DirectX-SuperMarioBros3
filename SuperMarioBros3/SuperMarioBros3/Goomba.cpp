#include "Goomba.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "MiscConst.h"

CGoomba::CGoomba()
{
	CGoomba::Init();
	SetState(GOOMBA_STATE_WALK);
	currentPhysicsState = GoombaState::Walk;
	enemyTag = EnemyTag::Goomba;
}

void CGoomba::Init()
{
	LoadAnimation();
	isEnabled = true;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(GOOMBA_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Goomba");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);


	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(GOOMBA_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
}

void CGoomba::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(GOOMBA_STATE_WALK, animationManager->Clone("ani-goomba-walk"));
	AddAnimation(GOOMBA_STATE_DIE, animationManager->Clone("ani-goomba-die"));
}

void CGoomba::Update(DWORD dt, CCamera* cam)
{
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	if (currentPhysicsState != GoombaState::Die)
	{
		velocity.x = normal.x * GOOMBA_SPEED;
	}
	else if (GetTickCount64() - startDeadTime > GOOMBA_DIE_TIME)
	{
		this->isEnabled = false;
		physiscBody->SetDynamic(false);
		physiscBody->SetGravity(0.0f);
		velocity.y = 0.0f;

	}
	physiscBody->SetVelocity(velocity);
}

void CGoomba::Render(CCamera* cam)
{
	switch (currentPhysicsState)
	{
		case GoombaState::Walk:
		{
			SetState(GOOMBA_STATE_WALK);
			break;
		}
		case GoombaState::Die:
		{
			SetState(GOOMBA_STATE_DIE);
			break;
		}
	}
	CGameObject::Render(cam);
}

void CGoomba::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Solid)
		{
			if (collisionEvent->nx != 0)
			{
				auto normal = physiscBody->GetNormal();
				normal.x = -normal.x;
				physiscBody->SetNormal(normal);
			}
		}
		else if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Misc && collisionBox->GetName().compare(FIRE_BALL_NAME) == 0)
		{
			if (collisionEvent->nx != 0)
			{
				CGoomba::OnDie();
			}
		}
	}
}

void CGoomba::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::RaccoonTail)
	{
		CGoomba::OnDie();
	}
}

void CGoomba::OnDie()
{
	currentPhysicsState = GoombaState::Die;
	auto v = physiscBody->GetVelocity();
	v.x = 0.0f;
	v.y = 0.0f;
	physiscBody->SetVelocity(v);
	physiscBody->SetGravity(0.0f);
	startDeadTime = GetTickCount64();

	SetRelativePositionOnScreen(D3DXVECTOR2(0, (GOOMBA_BBOX.y - GOOMBA_DIE_BBOX.y) * 0.5f));
	collisionBoxs->at(0)->SetSizeBox(GOOMBA_DIE_BBOX);
}
