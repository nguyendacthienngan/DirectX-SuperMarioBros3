#include "VenusFireBall.h"
#include "MiscConst.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "SceneManager.h"

CVenusFireBall::CVenusFireBall()
{
	this->SetTag(GameObjectTags::VenusFireBall);

	LoadAnimation();
	SetState(FIRE_BALL_ANIMATION);
	isEnabled = false;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(FIRE_BALL_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName(FIRE_BALL_NAME);
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);


	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(FIRE_BALL_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));

	pool = NULL;
}

CVenusFireBall::~CVenusFireBall()
{
	CGameObject::~CGameObject();
}

void CVenusFireBall::PhysicsUpdate(std::vector<LPGameObject>* coObjects)
{
	CGameObject::PhysicsUpdate(coObjects);
}

void CVenusFireBall::Update(DWORD dt, CCamera* cam)
{
	CGameObject::Update(dt, cam);
	if (transform.position.y > cam->GetBoundary().bottom)
	{
		if (pool != NULL)
			pool->Revoke(this);
	}
}

void CVenusFireBall::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(FIRE_BALL_ANIMATION, animationManager->Get("ani-fire-ball"));
}

void CVenusFireBall::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Solid
			|| collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Enemy)
		{
			if (collisionEvent->nx != 0)
			{
				if (pool != NULL)
					pool->Revoke(this);
			}
		}
	}
}

bool CVenusFireBall::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (MarioTag(tag) == true)
		return true;
	if (GiftTag(tag) == true)
		return false;
	return false;
}

void CVenusFireBall::LinkToPool(CObjectPool* pool)
{
	this->pool = pool;
}
