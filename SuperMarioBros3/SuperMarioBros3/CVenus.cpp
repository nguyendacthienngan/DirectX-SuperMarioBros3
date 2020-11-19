#include "CVenus.h"
#include "AnimationManager.h"
#include "VenusConst.h"
#include "FireBall.h"
#include "SceneManager.h"
#include "Ultis.h"
CVenus::CVenus()
{
	LoadAnimation();
	Init();
}

void CVenus::Init()
{
	isEnabled = true;
	SetState(VENUS_STATE_STAND);

	enemyTag = EnemyTag::Piranha;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(VENUS_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Venus");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));

	maxHeight = VENUS_HEIGHT;
	timeToStopDartOut = DART_OUT_TIME;
	timeToIdle = TIME_TO_IDLE;

	countFireBalls = 0;
	vectorShootFireBall = D3DXVECTOR2(0.0f, 0.0f);
	physiscBody->SetNormal(D3DXVECTOR2(1.0f, 1.0f));

}

void CVenus::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(VENUS_STATE_STAND, animationManager->Clone("ani-red-venus-fire-trap-stand"));
}

void CVenus::Update(DWORD dt, CCamera* cam)
{
	CPiranha::Update(dt, cam);

	// Nếu đang freeze thì quăng lửa
	if (isIdle == true)
	{
		countFireBalls++;
		auto normal = physiscBody->GetNormal();
		if (target != NULL)
			normal.x = (target->GetPosition() < this->transform.position) ? -1 : 1;
		physiscBody->SetNormal(normal);

		/*if (countFireBalls == 1) 
		{
			CFireBall* currentFireBall;
			currentFireBall = new CFireBall();
			currentFireBall->Enable(true);
			currentFireBall->SetPosition(transform.position);

			auto firePhyBody = currentFireBall->GetPhysiscBody();
			firePhyBody->SetGravity(0.0f);


			auto posVenus = transform.position + relativePositionOnScreen;
			posVenus.x += VENUS_BBOX.x * 0.5f * normal.x ;
			currentFireBall->SetPosition(posVenus);

			currentFireBall->SetCheckCollisionWithSolid(false);
			vectorShootFireBall.x = cos(SHOOT_FIRE_BALL_ANGLE);
			vectorShootFireBall.y = sin(SHOOT_FIRE_BALL_ANGLE);
			
			firePhyBody->SetVelocity(D3DXVECTOR2(FIRE_BALL_SPEED * normal.x * vectorShootFireBall.x, FIRE_BALL_SPEED * vectorShootFireBall.y));

			auto scene = CSceneManager::GetInstance()->GetActiveScene();
			scene->AddObject(currentFireBall);

		}*/
	}
	if (isIdle == false)
		countFireBalls = 0;
}

void CVenus::Render(CCamera* cam, int alpha)
{
	auto normal = physiscBody->GetNormal();
	SetScale(D3DXVECTOR2(-normal.x, 1.0f));
	CGameObject::Render(cam, alpha);
}
