#include "FireMario.h"
#include "AnimationManager.h"
#include "MarioConst.h"
#include "Misc.h"
#include "MiscConst.h"
#include "SceneManager.h"
#include "Ultis.h"

CFireMario::CFireMario()
{
	CMario::Init();
	marioStateTag = MarioStates::FireMario;
	Init();
	lastState = currentState;
	canCrouch = true;
	canAttack = true;

	countFireBall = 0;
	canAttackContinious = false;
	timeToNextAttack = 1000; // milisecond
	lastAttackTime = 0;
}

void CFireMario::Init()
{
	LoadAnimation();
	this->SetState(MARIO_STATE_IDLE); // Để tên đồng nhất với animation
}

void CFireMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-fire-mario-high-speed"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-fire-mario-walk"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-fire-mario-jump"));
	AddAnimation(MARIO_STATE_FLY, animationManager->Get("ani-fire-mario-high-jump"));
	AddAnimation(MARIO_STATE_FALL, animationManager->Get("ani-fire-mario-fall"));
	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-fire-mario-idle"));
	AddAnimation(MARIO_STATE_SKID, animationManager->Get("ani-fire-mario-skid"));
	AddAnimation(MARIO_STATE_CROUCH, animationManager->Get("ani-fire-mario-crouch"));
	AddAnimation(MARIO_STATE_ATTACK, animationManager->Get("ani-fire-mario-throw"), false);
}

void CFireMario::Render(CCamera* cam)
{
	CMario::Render(cam);
}

void CFireMario::Update(DWORD dt, CCamera* cam)
{
	CMario::Update(dt, cam);
}

void CFireMario::EndAnimation()
{
	if (currentState.compare(MARIO_STATE_ATTACK) == 0)
	{

		isAttack = false; 
		if (animations.find(lastState) == animations.end() || lastState == currentState) // Không kiếm được last state trong animation, đồng nghĩa với việc last state chưa được khởi tạo, còn nếu đc khởi tạo rồi thì mình set state theo cái state trước đó
		{
			lastState = MARIO_STATE_IDLE;
		}
		SetState(lastState); 
		if (countFireBall > 2)
		{
			DWORD now = GetTickCount();
			if (now - lastAttackTime >= timeToNextAttack) // Được attack típ
			{
				countFireBall = 0;
			}
		}
	}
}

void CFireMario::AddMiscToScene(CScene* scene)
{
}

void CFireMario::OnKeyDown(int KeyCode)
{
	CMario::OnKeyDown(KeyCode);
	if (isAttack == true)
	{
		countFireBall++; 

		DWORD now = GetTickCount();
		if (now - lastAttackTime < timeToNextAttack && countFireBall > 2)  
		{
			// Cannot attack until 1 second
			isAttack = false; 
			return;
		}
		if (now - lastAttackTime >= timeToNextAttack)
		{
			//do đến lần thứ 4, now > lastAttackTime rất nhiều và fireBall cx lớn hơn 2. Đáng ra nó phải quăng đc bóng. Nhưng, k có chỗ reset lại số bóng của nó
			countFireBall = 1; 
		}
			
		if (countFireBall <= 2)
		{
			CFireBall* currentFireBall;
			currentFireBall = new CFireBall();
			listFireBalls.push_back(currentFireBall);

			auto scene = CSceneManager::GetInstance()->GetActiveScene();
			scene->AddObject(currentFireBall);

			auto firePhyBody = currentFireBall->GetPhysiscBody();

			currentFireBall->Enable(true);
			auto normal = physiscBody->GetNormal();

			auto posMario = transform.position + relativePositionOnScreen;
			posMario.x += SUPER_MARIO_BBOX.x * 0.5f * normal.x;
			currentFireBall->SetPosition(posMario);

			firePhyBody->SetVelocity(D3DXVECTOR2(FIRE_BALL_SPEED * normal.x, 0));
			
			lastAttackTime = GetTickCount();

		}
	}
}

CFireMario::~CFireMario()
{
	for (auto fireBall : listFireBalls)
	{
		delete fireBall;
	}
}
