#include "ParaKoopa.h"
#include "AnimationManager.h"
#include "ParaKoopaConst.h"
#include "MiscConst.h"

CParaKoopa::CParaKoopa()
{
	LoadAnimation();
	Init();
}

void CParaKoopa::Init()
{
	SetState(PARAKOOPA_STATE_FLY);
	isEnabled = true;
	enemyTag = EnemyTag::ParaKoopa;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(KOOPA_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Para-Koopa");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(KOOPA_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(KOOPA_SPEED, 0.0f));

	isJump = false;
}

void CParaKoopa::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(PARAKOOPA_STATE_FLY, animationManager->Clone("ani-green-koopa-paratroopa-fly"));
}

void CParaKoopa::Render(CCamera* cam, int alpha)
{
	auto normal = physiscBody->GetNormal();
	SetScale(D3DXVECTOR2(-normal.x, normal.y));
	CGameObject::Render(cam, alpha);
}

void CParaKoopa::Update(DWORD dt, CCamera* cam)
{
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	velocity.x = normal.x * KOOPA_SPEED;
	
	if (isJump == false && IsOnGround() == true)
	{
		if (abs(jumpStartPosition) - abs(transform.position.y) <= KOOPA_HEIGHT)
		{
			jumpStartPosition = transform.position.y;
			velocity.y = -KOOPA_FORCE_Y;
			isJump = true;
		}
	}

	if (isJump == true)
	{
		isOnGround = false;

		if (abs(jumpStartPosition) - abs(transform.position.y) > KOOPA_HEIGHT)
		{
			velocity.y = KOOPA_FORCE_Y / 2;
		}
		if (velocity.y > 0)
		{
			isJump = false;
		}
	}
	physiscBody->SetVelocity(velocity);
}

void CParaKoopa::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
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
		
		//else if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Misc && collisionBox->GetName().compare(FIRE_BALL_NAME) == 0)
		//{
		//	if (collisionEvent->nx != 0 || collisionEvent->ny != 0)
		//	{
		//		/*ChangeToShell();
		//		auto normalPlayer = collisionBox->GetGameObjectAttach()->GetPhysiscBody()->GetNormal();
		//		auto normalKS = koopaShell->GetPhysiscBody()->GetNormal();
		//		normalKS.x = normalPlayer.x;
		//		koopaShell->GetPhysiscBody()->SetNormal(normalKS);
		//		koopaShell->SetIsHeadShotByFireball(true);
		//		koopaShell->OnDie();*/
		//	}
		//}
	}
}

void CParaKoopa::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	
}

void CParaKoopa::ChangeToKoopa()
{
	this->isEnabled = false;
	this->physiscBody->SetDynamic(false);
	this->collisionBoxs->at(0)->SetEnable(false);

	if (koopa != NULL)
	{
		koopa->Enable(true);
		koopa->SetPosition(transform.position);
	}
}

CGreenKoopa* CParaKoopa::GetKoopa()
{
	return koopa;
}

void CParaKoopa::SetKoopa(CGreenKoopa* koopa)
{
	this->koopa = koopa;
}

bool CParaKoopa::IsOnGround()
{
	return isOnGround;
}
