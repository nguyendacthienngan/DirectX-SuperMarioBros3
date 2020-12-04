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
	collisionBox->SetSizeBox(KOOPA_SHELL_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Para-Koopa");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	collisionBox->SetPosition(D3DXVECTOR2(0.0f, KOOPA_SHELL_BBOX.y /2 - DISTANCE_BETWEEN_PARA_KOOPA_PIVOT_AND_KOOPA_SHELL_PIVOT));
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

void CParaKoopa::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	velocity.x = normal.x * KOOPA_SPEED;
	
	if (isJump == false && IsOnGround() == true)
	{
		if (abs(jumpStartPosition) - abs(transform.position.y) <= PARA_KOOPA_HEIGHT)
		{
			jumpStartPosition = transform.position.y;
			velocity.y = -PARA_KOOPA_FORCE_Y;
			isJump = true;
		}
	}

	if (isJump == true)
	{
		isOnGround = false;

		if (abs(jumpStartPosition) - abs(transform.position.y) > PARA_KOOPA_HEIGHT)
		{
			velocity.y = PARA_KOOPA_FORCE_Y / 2;
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
}

void CParaKoopa::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	CEnemy::OnOverlappedEnter(selfCollisionBox, otherCollisionBox);
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

void CParaKoopa::OnDamaged(CGameObject* otherGO)
{
	this->isEnabled = false;
	this->physiscBody->SetDynamic(false);
	this->collisionBoxs->at(0)->SetEnable(false);
	if (koopa != NULL)
	{
		koopa->SetPosition(transform.position);
		koopa->OnDamaged(otherGO);
	}
}
