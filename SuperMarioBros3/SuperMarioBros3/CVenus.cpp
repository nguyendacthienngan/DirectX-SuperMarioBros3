#include "CVenus.h"
#include "AnimationManager.h"
#include "VenusConst.h"
#include "SceneManager.h"
#include "Ultis.h"
#include "FireBall.h"

CVenus::CVenus()
{
	CVenus::LoadAnimation();
	CVenus::Init();
}

void CVenus::Init()
{
	isEnabled = true;
	//SetState(VENUS_STATE_HEAD_DOWN);

	enemyTag = EnemyTag::Venus;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(VENUS_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Venus");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	physiscBody->SetNormal(D3DXVECTOR2(1.0f, 1.0f));

	maxHeight = VENUS_HEIGHT;
	timeToStopDartOut = DART_OUT_TIME;
	timeToIdle = TIME_TO_IDLE;

	countFireBalls = 0;
	vectorShootFireBall = D3DXVECTOR2(0.0f, 0.0f);

	for (int i = 0; i < 10; i++)
	{
		CFireBall* fireBall = new CFireBall();
		fireBall->LinkToPool(&fireBalls);
		fireBalls.Add(fireBall);
	}
}

void CVenus::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(VENUS_STATE_HEAD_UP, animationManager->Clone("ani-red-venus-fire-trap-headup"));
	AddAnimation(VENUS_STATE_HEAD_DOWN, animationManager->Clone("ani-red-venus-fire-trap-headdown"));
}

void CVenus::Update(DWORD dt, CCamera* cam)
{
	CPlant::Update(dt, cam);
	// Nếu đang freeze thì quăng lửa
	auto normal = physiscBody->GetNormal();
	if (target != NULL)
	{
		normal.x = (target->GetPosition().x < this->transform.position.x) ? -1 : 1;
		normal.y = (target->GetPosition().y < this->transform.position.y) ? -1 : 1;
	}
	physiscBody->SetNormal(normal);
	if (isIdle == true)
	{
		countFireBalls++;
		if (countFireBalls == 1) 
		{
			auto currentFireBall = fireBalls.Init();
			if (currentFireBall != NULL)
			{
				currentFireBall->SetPosition(transform.position);
				auto firePhyBody = currentFireBall->GetPhysiscBody();
				firePhyBody->SetGravity(0.0f);

				auto posVenus = transform.position + relativePositionOnScreen;
				posVenus.x += VENUS_BBOX.x * 0.5f * normal.x;
				currentFireBall->SetPosition(posVenus);

				currentFireBall->SetCheckCollisionWithSolid(false);
				vectorShootFireBall.x = cos(SHOOT_FIRE_BALL_ANGLE);
				vectorShootFireBall.y = sin(SHOOT_FIRE_BALL_ANGLE);

				firePhyBody->SetVelocity(D3DXVECTOR2(FIRE_BALL_SPEED * normal.x * vectorShootFireBall.x, FIRE_BALL_SPEED * vectorShootFireBall.y * normal.y));
			}
		}
	}
	if (isIdle == false)
		countFireBalls = 0;
}

void CVenus::Render(CCamera* cam, int alpha)
{
	auto normal = physiscBody->GetNormal();
	SetScale(D3DXVECTOR2(-normal.x, 1.0f));
	if (normal.y == 1)
		SetState(VENUS_STATE_HEAD_DOWN);
	else 
		SetState(VENUS_STATE_HEAD_UP);

	CGameObject::Render(cam, alpha);
}

CObjectPool CVenus::GetObjectPool()
{
	return fireBalls;
}
