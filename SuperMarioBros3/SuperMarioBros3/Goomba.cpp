#include "Goomba.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "MiscConst.h"
#include "SceneManager.h"

CGoomba::CGoomba()
{
	CGoomba::Init();
	SetState(GOOMBA_STATE_WALK);
	currentPhysicsState = GoombaState::Walk;
	enemyTag = EnemyTag::Goomba;
}

void CGoomba::Init()
{
	LoadAnimation();
	isEnabled = true;
	isJumpMaxHeight = false;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(GOOMBA_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Goomba");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);


	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(GOOMBA_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	beforeHitPosition = D3DXVECTOR2(0.0f, 0.0f);
}

void CGoomba::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(GOOMBA_STATE_WALK, animationManager->Clone("ani-goomba-walk"));
	AddAnimation(GOOMBA_STATE_DIE, animationManager->Clone("ani-goomba-die"));
	AddAnimation(GOOMBA_STATE_IDLE, animationManager->Clone("ani-goomba-idle"));
}

void CGoomba::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	if (currentPhysicsState == GoombaState::Walk)
	{
		velocity.x = normal.x * GOOMBA_SPEED;
	}
	else if (GetTickCount64() - startDeadTime > GOOMBA_DIE_TIME && currentPhysicsState == GoombaState::Die)
	{
		this->isEnabled = false;
		physiscBody->SetDynamic(false);
		physiscBody->SetGravity(0.0f);
		velocity.y = 0.0f;

	}
	physiscBody->SetVelocity(velocity);
}

void CGoomba::Render(CCamera* cam, int alpha)
{
	auto normal = physiscBody->GetNormal();
	SetScale(D3DXVECTOR2(1.0f, normal.y));
	switch (currentPhysicsState)
	{
		case GoombaState::Walk: 
		{
			SetState(GOOMBA_STATE_WALK);
			break;
		}
		case GoombaState::Die:
		{
			SetState(GOOMBA_STATE_DIE);
			break;
		}
		case GoombaState::HeadShot:
		{
			SetState(GOOMBA_STATE_HEADSHOT);
			break;
		}
	}
	CGameObject::Render(cam, alpha);
}

void CGoomba::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	CEnemy::OnCollisionEnter(selfCollisionBox, collisionEvents);
}

void CGoomba::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	CEnemy::OnOverlappedEnter(selfCollisionBox, otherCollisionBox);
}

void CGoomba::OnDie()
{
	auto v = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();

	if (isHeadShot == true)
	{
		this->collisionBoxs->at(0)->SetEnable(false);

		hitFX->SetStartPosition(this->transform.position);
		hitFX->SetStartHitTime(GetTickCount64());
		currentPhysicsState = GoombaState::HeadShot;
		v.x = GOOMBA_HIT_FORCE_X;
		normal.y = -1;
		countDeadCallback++;

		if (countDeadCallback == 1)
		{
			beforeHitPosition = transform.position;
			v.y = -GOOMBA_HIT_FORCE;
			auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
			activeScene->AddObject(hitFX);
			hitFX->Enable(true);
		}
	}
	else
	{
		currentPhysicsState = GoombaState::Die;

		v.x = 0.0f;
		v.y = 0.0f;
		
		countDeadCallback++;
		if (countDeadCallback == 1)
			startDeadTime = GetTickCount64();

		SetRelativePositionOnScreen(D3DXVECTOR2(0, (GOOMBA_BBOX.y - GOOMBA_DIE_BBOX.y) * 0.5f));
		collisionBoxs->at(0)->SetSizeBox(GOOMBA_DIE_BBOX);
		physiscBody->SetGravity(0.0f);

	}
	physiscBody->SetVelocity(v);
	physiscBody->SetNormal(normal);
}
