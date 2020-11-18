#include "CVenus.h"
#include "AnimationManager.h"
#include "VenusConst.h"
#include "FireBall.h"
#include "SceneManager.h"
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
		if (countFireBalls == 1)
		{
			CFireBall* currentFireBall;
			currentFireBall = new CFireBall();
			currentFireBall->Enable(true);
			currentFireBall->SetPosition(transform.position);


			/*auto posVenus = transform.position + relativePositionOnScreen;
			posMario.x += SUPER_MARIO_BBOX.x * 0.5f * normal.x;
			currentFireBall->SetPosition(posMario);*/

			vectorShootFireBall.x = cos(SHOOT_FIRE_BALL_ANGLE);
			vectorShootFireBall.x = -sin(SHOOT_FIRE_BALL_ANGLE);

			auto firePhyBody = currentFireBall->GetPhysiscBody();
			auto normal = firePhyBody->GetNormal();

			firePhyBody->SetVelocity(D3DXVECTOR2(FIRE_BALL_SPEED * normal.x * vectorShootFireBall.x, FIRE_BALL_SPEED * vectorShootFireBall.y));
			//firePhyBody->SetVelocity(D3DXVECTOR2(FIRE_BALL_SPEED * normal.x * vectorShootFireBall.x, 0.0f));

			auto scene = CSceneManager::GetInstance()->GetActiveScene();
			scene->AddObject(currentFireBall);
		}
	

	}
}
