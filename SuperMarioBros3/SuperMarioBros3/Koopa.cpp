#include "Koopa.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "KoopaConst.h"
#include "MiscConst.h"

CKoopa::CKoopa()
{
	LoadAnimation();
	Init();
	enemyTag = EnemyTag::Koopa;

}

void CKoopa::Init()
{
	LoadAnimation();
	SetState(KOOPA_STATE_MOVE);
	isEnabled = false;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(KOOPA_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Koopa");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);


	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(KOOPA_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
}

void CKoopa::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(KOOPA_STATE_MOVE, animationManager->Get("ani-red-koopa-troopa-move"));
	AddAnimation(KOOPA_STATE_WITH_DRAW, animationManager->Get("ani-red-koopa-troopa-with-draw"));
}

void CKoopa::Update(DWORD dt, CCamera* cam)
{
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();

	velocity.x = normal.x * KOOPA_SPEED;

	physiscBody->SetVelocity(velocity);
}

void CKoopa::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
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
		else if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Misc && collisionBox->GetName().compare(FIRE_BALL_NAME) == 0)
		{
			if (collisionEvent->nx != 0)
			{
				CKoopa::OnDie();
			}
		}
		else if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Player)
		{
			// Mario đạp lên đầu
			if (collisionEvent->ny != 0)
			{
				CKoopa::OnDie();
			}
		}
	}
}

void CKoopa::OnDie()
{
}
