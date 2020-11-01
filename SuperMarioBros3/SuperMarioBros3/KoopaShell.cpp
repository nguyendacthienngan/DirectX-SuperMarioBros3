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
	isRun = false;
	stopHold = false;
	isDead = false;
}

void CKoopaShell::Init()
{
	LoadAnimation();
	SetState(KOOPA_SHELL_STATE_IDLE);
	isEnabled = false;

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
	auto vel = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();

	if ((IsHolding() == true))
		physiscBody->SetGravity(0);
	else
		physiscBody->SetGravity(KOOPA_GRAVITY);

	if (canRun == true)
	{
		vel.x = KOOPA_SHELL_SPEED * normal.x;
		isRun = true;
	}
	else if (isRun == false)
	{
		vel.x = 0.0f;
	}

	physiscBody->SetVelocity(vel);
	DebugOut(L"KoopaShell Position  %f, %f \n", transform.position.x, transform.position.y);
	DebugOut(L"KoopaShell Gravity  %f \n", physiscBody->GetGravity());
	DebugOut(L"KoopaShell Velocity  %f, %f \n", vel.x, vel.y);

}

void CKoopaShell::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
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
				DebugOut(L"NORMAL : %d, %d \n", normal.x, normal.y);
			}
		}
		else if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Misc && collisionBox->GetName().compare(FIRE_BALL_NAME) == 0)
		{
			// Nếu mai rùa bị đạn bắn là nó lật lại (-1) r bị văng đi khỏi ground lun : HEADSHOT
			if (collisionEvent->nx != 0)
			{
				CKoopaShell::OnDie();
				//headShot = true;
			}
		}
	}
}

void CKoopaShell::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::RaccoonTail)
	{
		// Chỉ khi bị đuôi quật nó mới set lại -1 r văng đi (chưa văng khỏi ground)
		// cần xử lý lại việc chết cho hợp lý
		CKoopaShell::OnDie();
		headShot = true;
		auto normal = physiscBody->GetNormal();
		normal.x = otherCollisionBox->GetGameObjectAttach()->GetPhysiscBody()->GetNormal().x;
		physiscBody->SetNormal(normal);
	}
}

void CKoopaShell::OnDie()
{
	auto normal = physiscBody->GetNormal();
	normal.y = -1;
	physiscBody->SetNormal(normal);
	physiscBody->SetGravity(0.0f);

	// Chỗ này giúp tạo hiệu ứng văng đi 
	if (headShot)
	{
		auto v = physiscBody->GetVelocity();
		v.y = -KOOPA_SHELL_DEFLECT;
		v.x = KOOPA_SHELL_DEFLECT_X * normal.x;
		physiscBody->SetVelocity(v);
	}
	isDead = true;

}

void CKoopaShell::SetHoldablePosition(D3DXVECTOR2 pos)
{
	SetPosition(pos);
}

void CKoopaShell::Release()
{
	CHoldable::Release();
	// Thả ra là mai rùa chạy !
	isEnabled = true;
	stopHold = true;
	this->canRun = true;
	physiscBody->SetGravity(KOOPA_GRAVITY);
	physiscBody->SetNormal(normal); // Sẽ set lại normal theo hướng bị thả ra dựa vào normal bên class cha Holdable giữ
}

D3DXVECTOR2 CKoopaShell::GetHoldableCollisionBox()
{
	return collisionBoxs->at(0)->GetSizeBox();
}

bool CKoopaShell::IsHolding()
{
	stopHold = !CHoldable::IsHolding();
	return CHoldable::IsHolding();
}

void CKoopaShell::Render(CCamera* cam)
{
	auto normal = physiscBody->GetNormal();
	SetScale(normal);
	if (isRun == true)
		SetState(KOOPA_SHELL_STATE_RUN);
	else
		SetState(KOOPA_SHELL_STATE_IDLE);
	CGameObject::Render(cam);
}

bool CKoopaShell::IsRunning()
{
	return isRun || canRun;
}

void CKoopaShell::SetRun()
{
	this->canRun = true;
}

void CKoopaShell::SetStopRun()
{
	isRun = false;
	canRun = false;
	auto vel = physiscBody->GetVelocity();
	vel.x = 0.0f;
	physiscBody->SetVelocity(vel);
	DebugOut(L"Stop run \n");
}
