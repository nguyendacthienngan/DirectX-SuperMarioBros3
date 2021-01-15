#include "ParaKoopa.h"
#include "ParaKoopaConst.h"
#include "MiscConst.h"

CParaKoopa::CParaKoopa()
{
	LoadAnimation();
	Init();
}

void CParaKoopa::Init()
{
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
}

void CParaKoopa::Render(CCamera* cam, int alpha)
{
	auto normal = physiscBody->GetNormal();
	SetScale(D3DXVECTOR2(-normal.x, normal.y));
	CGameObject::Render(cam, alpha);
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

CKoopa* CParaKoopa::GetKoopa()
{
	return koopa;
}

void CParaKoopa::SetKoopa(CKoopa* koopa)
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
