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
	upsideDown = false;
	isWithDraw == false;
	canWithDraw == false;
}

void CKoopaShell::Init()
{
	LoadAnimation();
	SetState(KOOPA_SHELL_STATE_IDLE);
	isEnabled = false;
	timeStartWithDraw = 0;
	timeStartCanWithDraw = 0;
	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(KOOPA_SHELL_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Koopa-Shell");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	startDeadTime = 0;
	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(KOOPA_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
}

void CKoopaShell::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(KOOPA_SHELL_STATE_IDLE, animationManager->Get("ani-red-koopa-troopa-shell-idle"));
	AddAnimation(KOOPA_SHELL_STATE_RUN, animationManager->Get("ani-red-koopa-troopa-shell-run"));
	AddAnimation(KOOPA_SHELL_STATE_WITHDRAW, animationManager->Get("ani-red-koopa-troopa-with-draw"));
}

void CKoopaShell::Update(DWORD dt, CCamera* cam)
{
	auto vel = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	if (isDead == true)
	{
		this->isEnabled = false;
		physiscBody->SetDynamic(false);
		physiscBody->SetGravity(0.0f);
		vel.y = 0.0f;
		/*if (GetTickCount64() - startDeadTime > KOOPA_DIE_TIME && startDeadTime != 0)
		{
			this->isEnabled = false;
			physiscBody->SetDynamic(false);
			physiscBody->SetGravity(0.0f);
			vel.y = 0.0f;
		}*/
	}
	else
	{
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
	}

	WithDrawProcess();

	physiscBody->SetNormal(normal);
	physiscBody->SetVelocity(vel);
}

void CKoopaShell::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	CEnemy::OnCollisionEnter(selfCollisionBox, collisionEvents);

	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Solid)
		{
			auto normal = physiscBody->GetNormal();
			if (collisionEvent->nx != 0)
			{
				normal.x = -normal.x;
				physiscBody->SetNormal(normal);
			}
		}
		else if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Misc && collisionBox->GetName().compare(FIRE_BALL_NAME) == 0)
		{
			// Nếu mai rùa bị đạn bắn là nó lật lại (-1) r bị văng đi khỏi ground lun : HEADSHOT
			if (collisionEvent->nx != 0 || collisionEvent->ny != 0)
			{
				CKoopaShell::OnDie();
				//headShot = true;
			}
		}
		else if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Enemy)
		{
			if (collisionEvent->nx != 0 || collisionEvent->ny != 0)
			{
				if (isRun == true || IsHolding() == true)
				{
					auto enemyObj = static_cast<CEnemy*>(collisionBox->GetGameObjectAttach());
					enemyObj->OnDie();
				}
				
			}
		}
	}
}

void CKoopaShell::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	CEnemy::OnOverlappedEnter(selfCollisionBox, otherCollisionBox);

	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::RaccoonTail)
	{
		// Chỉ khi bị đuôi quật nó mới set lại -1 r văng đi (chưa văng khỏi ground)
		// cần xử lý lại việc chết cho hợp lý
		CKoopaShell::OnDie();
	}
	else if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Misc && otherCollisionBox->GetName().compare(FIRE_BALL_NAME) == 0)
	{
		CKoopaShell::OnDie();
	}
	else if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Enemy)
	{
		if (isRun == true || IsHolding() == true)
		{
			auto enemyObj = static_cast<CEnemy*>(otherCollisionBox->GetGameObjectAttach());
			enemyObj->OnDie();
		}
	}
}

void CKoopaShell::OnDie()
{
	auto normal = physiscBody->GetNormal();
	physiscBody->SetGravity(0.0f);
	upsideDown = true;
	if (headShot)
	{
		auto v = physiscBody->GetVelocity();
		v.y = -KOOPA_SHELL_DEFLECT;
		v.x = KOOPA_SHELL_DEFLECT_X * normal.x;
		physiscBody->SetVelocity(v);
		isDead = true;
		startDeadTime = GetTickCount64();
	}
}

void CKoopaShell::SetKoopa(CKoopa* koopa)
{
	this->koopa = koopa;
}

CKoopa* CKoopaShell::GetKoopa()
{
	return koopa;
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

void CKoopaShell::Render(CCamera* cam, int alpha)
{
	auto normal = physiscBody->GetNormal();
	if (upsideDown == true)
	{
		normal.y = -1.0f;
		physiscBody->SetNormal(normal);
		SetScale(D3DXVECTOR2(1.0f, -1.0f));
	}
	else
		SetScale(D3DXVECTOR2(1.0f, 1.0f));
	if (isRun == true)
		SetState(KOOPA_SHELL_STATE_RUN);
	else
	{
		if (isWithDraw == true)
			SetState(KOOPA_SHELL_STATE_WITHDRAW);
		else
			SetState(KOOPA_SHELL_STATE_IDLE);
	}
	CGameObject::Render(cam, alpha);
}

void CKoopaShell::WithDrawProcess()
{
	// Chỉ cần mai rùa không chạy thì mai rùa có thể rúc đầu (withdraw) sau KOOPA_MUST_START_WITH_DRAW_TIME
	canWithDraw = (!IsRunning());

	// Bị 2 vấn đề, 1 là bị chuyển liền qua with-draw
	// 2 là k set lai được isWithdraw = false
	if (canWithDraw == true && timeStartCanWithDraw != 0)
	{
		timeStartCanWithDraw = GetTickCount64();
	}

	// Kết thúc withdraw animation
	if (isWithDraw == true && GetTickCount64() - timeStartWithDraw > KOOPA_WITH_DRAW_TIME)
	{
		isWithDraw = false;
		timeStartWithDraw = 0;
	}

	// Bắt đầu withdraw animation
	if (canWithDraw == true && GetTickCount64() - timeStartCanWithDraw > KOOPA_MUST_START_WITH_DRAW_TIME)
	{
		timeStartWithDraw = GetTickCount64();
		timeStartCanWithDraw = 0;
		isWithDraw = true;
	}

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
