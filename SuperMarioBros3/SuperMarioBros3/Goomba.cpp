#include "Goomba.h"
#include "AnimationManager.h"
#include "Ultis.h"

CGoomba::CGoomba()
{
	CGoomba::Init();
	//distanceToMove = 200;
	SetState(GOOMBA_STATE_WALK);
	currentPhysicsState = GoombaState::Walk;
}

void CGoomba::Init()
{
	LoadAnimation();
	isEnabled = false;

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
	//DebugOut(L"Goomba Position Y: %f \n", transform.position.y);
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	if (currentPhysicsState != GoombaState::Die)
	{
		velocity.x = normal.x * GOOMBA_SPEED;
	}
	else
	{
		// Tạm thời thui, sau này còn xét kiểu chết khác nhau
		// Và mình sẽ cho nó time để die riêng
		// Sau khi hết time là nó tự disable
		velocity.x = 0;
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
				normal.x = -1;
				physiscBody->SetNormal(normal);
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

}
