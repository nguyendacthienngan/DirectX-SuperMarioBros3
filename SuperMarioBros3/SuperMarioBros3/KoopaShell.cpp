#include "KoopaShell.h"
#include "KoopaConst.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "MiscConst.h"
CKoopaShell::CKoopaShell()
{
	LoadAnimation();
	Init();
	enemyTag = EnemyTag::KoopaShell;
}

void CKoopaShell::Init()
{
	LoadAnimation();
	SetState(KOOPA_SHELL_STATE_IDLE);
	isEnabled = true;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(KOOPA_SHELL_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Koopa-Shell");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);


	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(KOOPA_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
}

void CKoopaShell::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(KOOPA_SHELL_STATE_IDLE, animationManager->Get("ani-red-koopa-troopa-shell-idle"));
	AddAnimation(KOOPA_SHELL_STATE_RUN, animationManager->Get("ani-red-koopa-troopa-shell-run"));
}

void CKoopaShell::Update(DWORD dt, CCamera* cam)
{
	//DebugOut(L"KoopaShell Position.y %f \n", transform.position.y);
}

void CKoopaShell::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Misc && collisionBox->GetName().compare(FIRE_BALL_NAME) == 0)
		{
			if (collisionEvent->nx != 0)
			{
				CKoopaShell::OnDie();
			}
		}
		else if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Player)
		{
			// Mario đạp lên đầu
			if (collisionEvent->ny != 0)
			{
				CKoopaShell::OnDie();
			}
		}
	}
}

void CKoopaShell::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::RaccoonTail)
	{
		DebugOut(L"Koopa Shell Died \n");
		CKoopaShell::OnDie();
	}
}

void CKoopaShell::OnDie()
{
	auto normal = physiscBody->GetNormal();
	normal.y = -1;
	physiscBody->SetNormal(normal);
	auto v = physiscBody->GetVelocity();
	v.y = -1.0f;
	physiscBody->SetVelocity(v);
	physiscBody->SetGravity(0.0f);
}

void CKoopaShell::Render(CCamera* cam)
{
	auto normal = physiscBody->GetNormal();
	SetScale(normal);
	CGameObject::Render(cam);
}
