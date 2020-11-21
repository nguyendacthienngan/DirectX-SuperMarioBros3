﻿#include "MarioFireBall.h"
#include "MiscConst.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "SceneManager.h"

CMarioFireBall::CMarioFireBall()
{
	this->SetTag(GameObjectTags::MarioFireBall);

	LoadAnimation();
	SetState(FIRE_BALL_ANIMATION);

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(FIRE_BALL_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName(FIRE_BALL_NAME);
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);


	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(FIRE_BALL_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	physiscBody->SetBounceForce(FIRE_BALL_BOUNCE_FORCE);

	pool = NULL;
}

CMarioFireBall::~CMarioFireBall()
{
	CGameObject::~CGameObject();
}

void CMarioFireBall::PhysicsUpdate(std::vector<LPGameObject>* coObjects)
{
	CGameObject::PhysicsUpdate(coObjects);
}

void CMarioFireBall::Update(DWORD dt, CCamera* cam)
{
	CGameObject::Update(dt, cam);
	if (transform.position.y > cam->GetBoundary().bottom)
	{
		if (pool != NULL)
			pool->Revoke(this);
	}
	if (transform.position.x < cam->GetPositionCam().x || transform.position.x > cam->GetPositionCam().x + cam->GetWidthCam())
	{
		if (pool != NULL)
			pool->Revoke(this);
	}
}

void CMarioFireBall::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(FIRE_BALL_ANIMATION, animationManager->Get("ani-fire-ball"));
}

void CMarioFireBall::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Solid || collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::QuestionBlock
			|| collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Enemy || collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Brick)
		{
			if (collisionEvent->nx != 0)
			{
				if (pool != NULL)
					pool->Revoke(this);
			}
			
		}
	}
}

bool CMarioFireBall::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (tag == GameObjectTags::MarioFireBall || tag == GameObjectTags::VenusFireBall)
		return false;
	if (MarioTag(tag) == true)
		return false;
	if (GiftTag(tag) == true)
		return false;
	return true;
}

void CMarioFireBall::LinkToPool(CObjectPool* pool)
{
	this->pool = pool;
}