﻿#include "Goomba.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "GoombaConst.h"

CGoomba::CGoomba()
{
	CGoomba::Init();
	distanceToMove = 200;
	SetState(GOOMBA_STATE_WALK);

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
	AddAnimation(GOOMBA_STATE_IDLE, animationManager->Clone("ani-goomba-idle"));
	AddAnimation(GOOMBA_STATE_WALK, animationManager->Clone("ani-goomba-walk"));
	AddAnimation(GOOMBA_STATE_DIE, animationManager->Clone("ani-goomba-die"));
}

void CGoomba::Update(DWORD dt, CCamera* cam)
{
	//DebugOut(L"Goomba Position Y: %f \n", transform.position.y);
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();

	velocity.x = normal.x * GOOMBA_SPEED;

	physiscBody->SetVelocity(velocity);
}

void CGoomba::Render(CCamera* cam)
{
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