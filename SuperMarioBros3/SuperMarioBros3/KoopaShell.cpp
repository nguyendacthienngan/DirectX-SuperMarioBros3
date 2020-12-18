#include "KoopaShell.h"
#include "KoopaConst.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "MiscConst.h"
#include "SceneManager.h"
CKoopaShell::CKoopaShell()
{
	Init();
	enemyTag = EnemyTag::KoopaShell;
	isRun = false;
	stopHold = false;
	isDead = false;
	upsideDown = false;
	isWithDraw == false;
	canWithDraw == false;
	isReleaseFromHigher = true;
	countWithDraw = 0;
	countShakingTime = 0;
	timeStartHeadShot = 0;
	centerPosition = D3DXVECTOR2(0.0f, 0.0f);
}

void CKoopaShell::Init()
{
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
}

void CKoopaShell::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto vel = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	if (isHeadShot == true || isHeadShotByFireBall == true)
	{
		if (GetTickCount64() - timeStartHeadShot >= KOOPA_HEAD_SHOT_TIME)
		{
			isHeadShot = false;
			isHeadShotByFireBall = false;
			countDeadCallback = 0;
			vel.x = 0.0f;
		}
	}
	else
	{
		if ((IsHolding() == true) || isReleaseFromHigher == false)
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

	physiscBody->SetNormal(normal);
	physiscBody->SetVelocity(vel);
	WithDrawProcess();
}

void CKoopaShell::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	CEnemy::OnCollisionEnter(selfCollisionBox, collisionEvents);

}

void CKoopaShell::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	CEnemy::OnOverlappedEnter(selfCollisionBox, otherCollisionBox);
}

void CKoopaShell::OnDie()
{
	auto normal = physiscBody->GetNormal();
	upsideDown = true;
	if (isHeadShot || isHeadShotByFireBall)
	{
		countDeadCallback++;

		if (isReleaseFromHigher == false)
		{
			isReleaseFromHigher = true;
			physiscBody->SetGravity(KOOPA_GRAVITY);
		}
		if (countDeadCallback == 1)
		{
			timeStartHeadShot = GetTickCount64();

			auto v = physiscBody->GetVelocity();
			v.y = -KOOPA_SHELL_DEFLECT;
			auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
			activeScene->AddObject(hitFX);
			hitFX->SetStartPosition(this->transform.position);
			hitFX->SetStartHitTime(GetTickCount64());
			hitFX->Enable(true);

			if (isHeadShotByFireBall == true)
			{
				// Off the cliffs
				this->collisionBoxs->at(0)->SetEnable(false);
			}
			physiscBody->SetVelocity(v);
		}
	}
	else
		physiscBody->SetGravity(0.0f);

}

void CKoopaShell::CollisionWithRaccoonTail(CGameObject* gO)
{
	isHeadShot = true;
	auto normal = physiscBody->GetNormal();
	normal.x = -gO->GetPhysiscBody()->GetNormal().x;
	physiscBody->SetNormal(normal);
	CKoopaShell::OnDie();
}

void CKoopaShell::CollisionWithFireBall()
{
	isHeadShotByFireBall = true;
	CKoopaShell::OnDie();
}

void CKoopaShell::CollisionWithOtherEnemy(CollisionEvent* cE, CCollisionBox* cO)
{
	if (cE->nx != 0 || cE->ny != 0)
	{
		if (isRun == true || IsHolding() == true)
		{
			auto enemyObj = static_cast<CEnemy*>(cO->GetGameObjectAttach());
			enemyObj->SetIsHeadShot(true);
			enemyObj->OnDie();
			CollisionWithFireBall();
		}
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

void CKoopaShell::Release(bool isFromHigher)
{
	CHoldable::Release(isFromHigher);
	isReleaseFromHigher = isFromHigher;
	// Thả ra là mai rùa chạy !
	isEnabled = true;
	stopHold = true;
	this->canRun = true;
	// Nếu là small mario thì KOOPA_GRAVITY làm koopa shell đẩy xuống nhiều quá  / lố ground, rớt khỏi ground
	// Nên sẽ set gravity = 0 :( nhưng thả ra mà trúng quái thì trừ v mà còn gravity = 0 nên bay lên trời
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
	countWithDraw++;
	// Chỉ cần mai rùa không chạy VÀ ĐANG KHÔNG BỊ CẦM thì mai rùa có thể rúc đầu (withdraw) sau KOOPA_MUST_START_WITH_DRAW_TIME
	// Chỉ withdraw 1 lần sau khi đứng yên thôi, lúc đó chuyển lại cho koopa và khi koopa bị cụng đầu rồi mới tính lại
	if (isRun == true || canRun == true || IsHolding() == true)
		return;
	if (isRun == false|| canRun == false )
	{
		if (countWithDraw == 1)
			canWithDraw = true;
		if (canWithDraw == true && timeStartCanWithDraw == 0)
		{
			centerPosition = transform.position;
			timeStartCanWithDraw = GetTickCount64();
		}
	}

	// Kết thúc withdraw animation
	if (isWithDraw == true && GetTickCount64() - timeStartWithDraw > KOOPA_WITH_DRAW_TIME && timeStartWithDraw != 0)
	{
		isWithDraw = false;
		canWithDraw = false;
		timeStartWithDraw = 0;
		timeStartCanWithDraw = 0;
		countWithDraw = 0;
		SetRelativePositionOnScreen(D3DXVECTOR2(0.0f, 0.0f));
		koopa->ChangeBackToKoopa();
	}

	// Bắt đầu withdraw animation
	if (canWithDraw == true && GetTickCount64() - timeStartCanWithDraw > KOOPA_MUST_START_WITH_DRAW_TIME)
	{
		if (timeStartWithDraw == 0 && timeStartCanWithDraw != 0)
		{
			isWithDraw = true;
			timeStartWithDraw = GetTickCount64();
		}
	}

	if (isWithDraw == true)
	{
		countShakingTime++;
		auto velocity = physiscBody->GetVelocity();
		D3DXVECTOR2 relPos = D3DXVECTOR2(0.0f, 0.0f);
		relPos.x = rand() % SHAKING_AMPLITUDE - SHAKING_AMPLITUDE;
		SetRelativePositionOnScreen(relPos);
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

	isWithDraw = false;
	canWithDraw = false;
	timeStartWithDraw = 0;
	timeStartCanWithDraw = 0;
	countWithDraw = 0;
}
