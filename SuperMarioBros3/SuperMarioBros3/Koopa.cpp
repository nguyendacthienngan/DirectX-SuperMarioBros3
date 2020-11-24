#include "Koopa.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "MiscConst.h"

CKoopa::CKoopa()
{
	enemyTag = EnemyTag::Koopa;
}

void CKoopa::Init()
{
	SetState(KOOPA_STATE_MOVE);
	isEnabled = true;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(KOOPA_SHELL_ON_KOOPA_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Koopa");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	collisionBox->SetPosition(D3DXVECTOR2(0.0f, KOOPA_SHELL_ON_KOOPA_BBOX.y / 2 - DISTANCE_BETWEEN_KOOPA_PIVOT_AND_KOOPA_SHELL_PIVOT));
	this->collisionBoxs->push_back(collisionBox);
	
	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(KOOPA_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
}

void CKoopa::LoadAnimation()
{
}

void CKoopa::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
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
}

void CKoopa::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	CEnemy::OnOverlappedEnter(selfCollisionBox, otherCollisionBox);
}

void CKoopa::ChangeToShell()
{
	this->isEnabled = false;
	this->physiscBody->SetDynamic(false);
	this->collisionBoxs->at(0)->SetEnable(false);
	if (koopaShell != NULL)
	{
		koopaShell->Enable(true);
		koopaShell->SetPosition(transform.position);
	}
}

void CKoopa::OnDie()
{
	
}

void CKoopa::OnDamaged(CGameObject* otherGO)
{
	ChangeToShell();
	auto normalPlayer = otherGO->GetPhysiscBody()->GetNormal();
	auto normalKS = koopaShell->GetPhysiscBody()->GetNormal();
	normalKS.x = normalPlayer.x;
	koopaShell->GetPhysiscBody()->SetNormal(normalKS);
	koopaShell->SetIsHeadShotByFireball(true);
	koopaShell->OnDie();
}


void CKoopa::ChangeBackToKoopa()
{
	this->isEnabled = true;
	this->physiscBody->SetDynamic(true);
	this->collisionBoxs->at(0)->SetEnable(true);
	if (koopaShell != NULL)
	{
		koopaShell->Enable(false);
		transform.position = koopaShell->GetPosition();
		transform.position.y -= (KOOPA_BBOX.y - koopaShell->GetCollisionBox()->at(0)->GetSizeBox().y )*0.5f;
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

KoopaType CKoopa::GetKoopaType()
{
	return koopaType;
}

