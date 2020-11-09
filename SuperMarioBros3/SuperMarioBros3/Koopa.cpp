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
	isEnabled = true;

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
	AddAnimation(KOOPA_STATE_MOVE, animationManager->Clone("ani-red-koopa-troopa-move"));
	AddAnimation(KOOPA_STATE_WITH_DRAW, animationManager->Clone("ani-red-koopa-troopa-with-draw"));
}

void CKoopa::Update(DWORD dt, CCamera* cam)
{
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	velocity.x = normal.x * KOOPA_SPEED;

	physiscBody->SetVelocity(velocity);
}

void CKoopa::Render(CCamera* cam)
{
	auto normal = physiscBody->GetNormal();
	
	SetScale(D3DXVECTOR2(-normal.x, normal.y));
	CGameObject::Render(cam);
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
				normal.x = -normal.x;
				physiscBody->SetNormal(normal);
			}
		}
		else if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Misc && collisionBox->GetName().compare(FIRE_BALL_NAME) == 0)
		{
			if (collisionEvent->nx != 0)
			{
				ChangeToShell();
				koopaShell->OnDie();
			}
		}
	}
}

void CKoopa::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::RaccoonTail)
	{
		// Chỉ khi bị đuôi quật nó mới set lại -1 r văng đi (chưa văng khỏi ground)
		// cần xử lý lại việc chết cho hợp lý
		ChangeToShell();
		koopaShell->OnDie();
	}
}

void CKoopa::ChangeToShell()
{
	this->isEnabled = false;
	this->physiscBody->SetDynamic(false);
	if (koopaShell != NULL)
	{
		koopaShell->Enable(true);
		koopaShell->SetPosition(transform.position);
	}
}

void CKoopa::OnDie()
{

}

void CKoopa::SetBoundary(float boundLeft, float boundRight)
{
	boundaryLeft = boundLeft;
	boundaryRight = boundRight;
}

float CKoopa::GetBoundaryLeft()
{
	return 0.0f;
}

float CKoopa::GetBoundaryRight()
{
	return 0.0f;
}

void CKoopa::SetKoopaShell(CKoopaShell* koopaShell)
{
	this->koopaShell = koopaShell;
}

CKoopaShell* CKoopa::GetKoopaShell()
{
	return koopaShell;
}

