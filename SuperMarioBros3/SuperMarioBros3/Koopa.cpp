#include "Koopa.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "KoopaConst.h"
#include "MiscConst.h"

CKoopa::CKoopa()
{
	//LoadAnimation();
	//Init();
	enemyTag = EnemyTag::Koopa;
}

void CKoopa::Init()
{
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
}

void CKoopa::Update(DWORD dt, CCamera* cam)
{
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	velocity.x = normal.x * KOOPA_SPEED;

	physiscBody->SetVelocity(velocity);
}

void CKoopa::Render(CCamera* cam, int alpha)
{
	auto normal = physiscBody->GetNormal();
	
	SetScale(D3DXVECTOR2(-normal.x, normal.y));
	CGameObject::Render(cam, alpha);
}

void CKoopa::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	CEnemy::OnCollisionEnter(selfCollisionBox, collisionEvents);

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
			if (collisionEvent->nx != 0 || collisionEvent->ny != 0)
			{
				ChangeToShell();
				auto normalPlayer = collisionBox->GetGameObjectAttach()->GetPhysiscBody()->GetNormal();
				auto normalKS = koopaShell->GetPhysiscBody()->GetNormal();
				normalKS.x = normalPlayer.x;
				koopaShell->GetPhysiscBody()->SetNormal(normalKS);
				koopaShell->SetIsHeadShotByFireball(true);
				koopaShell->OnDie();
			}
		}
	}
}

void CKoopa::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	CEnemy::OnOverlappedEnter(selfCollisionBox, otherCollisionBox);

	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::RaccoonTail)
	{
		// Chỉ khi bị đuôi quật nó mới set lại -1 r văng đi (chưa văng khỏi ground)
		// cần xử lý lại việc chết cho hợp lý
		ChangeToShell();
		auto normalPlayer = otherCollisionBox->GetGameObjectAttach()->GetPhysiscBody()->GetNormal();
		auto normalKS = koopaShell->GetPhysiscBody()->GetNormal();
		normalKS.x = normalPlayer.x;
		koopaShell->GetPhysiscBody()->SetNormal(normalKS);
		koopaShell->SetIsHeadShot(true);
		koopaShell->OnDie();
	}
	else if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Misc && otherCollisionBox->GetName().compare(FIRE_BALL_NAME) == 0)
	{
		// Khi đụng trúng quái fireball có biến mất k?
		ChangeToShell();
		auto normalPlayer = otherCollisionBox->GetGameObjectAttach()->GetPhysiscBody()->GetNormal();
		auto normalKS = koopaShell->GetPhysiscBody()->GetNormal();
		normalKS.x = normalPlayer.x;
		koopaShell->GetPhysiscBody()->SetNormal(normalKS);
		koopaShell->SetIsHeadShotByFireball(true);
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

void CKoopa::ChangeBackToKoopa()
{
	this->isEnabled = true;
	this->physiscBody->SetDynamic(true);
	if (koopaShell != NULL)
	{
		koopaShell->Enable(false);
		transform.position = koopaShell->GetPosition();
		transform.position.y -= (collisionBoxs->at(0)->GetSizeBox().y  - koopaShell->GetCollisionBox()->at(0)->GetSizeBox().y )*0.5f;
	}
}

void CKoopa::SetKoopaShell(CKoopaShell* koopaShell)
{
	this->koopaShell = koopaShell;
}

CKoopaShell* CKoopa::GetKoopaShell()
{
	return koopaShell;
}

