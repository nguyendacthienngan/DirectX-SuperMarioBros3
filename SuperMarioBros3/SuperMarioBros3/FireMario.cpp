#include "FireMario.h"
#include "AnimationManager.h"
#include "MarioConst.h"
#include "Misc.h"
#include "MiscConst.h"
#include "SceneManager.h"
#include "Ultis.h"
#include "MarioFireBall.h"

CFireMario::CFireMario()
{
	CMario::Init();
	CFireMario::Init();
	CFireMario::LoadAnimation();
}

void CFireMario::Init()
{
	this->SetState(MARIO_STATE_IDLE); // Để tên đồng nhất với animation

	collisionBoxs->at(0)->SetName("Fire-Mario");

	marioStateTag = MarioStates::FireMario;
	lastState = currentState;
	canCrouch = true;
	canAttack = true;

	countFireBall = 0;
	canAttackContinious = false;
	timeToNextAttack = 1000; // milisecond
	lastAttackTime = 0;
	isJumpAttack = false;

	for (int i = 0; i < 2; i++)
	{
		CMarioFireBall* fireBall = new CMarioFireBall();
		fireBall->LinkToPool(&fireBalls);
		fireBalls.Add(fireBall);
	}
}

void CFireMario::Clear()
{
}

void CFireMario::LoadAnimation()
{
	CMario::LoadAnimation();
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-fire-mario-run"));
	AddAnimation(MARIO_STATE_HIGH_SPEED, animationManager->Get("ani-fire-mario-high-speed"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-fire-mario-walk"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-fire-mario-jump"));
	AddAnimation(MARIO_STATE_FLY, animationManager->Get("ani-fire-mario-high-jump"));
	AddAnimation(MARIO_STATE_FALL, animationManager->Get("ani-fire-mario-fall"));
	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-fire-mario-idle"));
	AddAnimation(MARIO_STATE_SKID, animationManager->Get("ani-fire-mario-skid"));
	AddAnimation(MARIO_STATE_CROUCH, animationManager->Get("ani-fire-mario-crouch"));
	AddAnimation(MARIO_STATE_ATTACK, animationManager->Get("ani-fire-mario-throw"), false);
	AddAnimation(MARIO_STATE_JUMP_ATTACK, animationManager->Get("ani-fire-mario-jump-throw"), false);
	AddAnimation(MARIO_STATE_HOLD_MOVE, animationManager->Get("ani-fire-mario-hold"));
	AddAnimation(MARIO_STATE_HOLD_IDLE, animationManager->Get("ani-fire-mario-hold-idle"));
	AddAnimation(MARIO_STATE_KICK, animationManager->Get("ani-fire-mario-kick"), false);
	AddAnimation(MARIO_STATE_IDLE_FRONT, animationManager->Get("ani-fire-mario-idle-front"));
	AddAnimation(MARIO_STATE_HOLD_JUMP, animationManager->Get("ani-fire-mario-hold-jump"));

}

void CFireMario::Render(CCamera* cam, int alpha)
{
	CMario::Render(cam, alpha);
}

void CFireMario::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	CMario::Update(dt, cam, uiCam);
	if (isAttack == true) 
	{
		if (isOnGround == true && isJump == false) 
			currentPhysicsState.move = MoveOnGroundStates::Attack;
		else
		{
			// S + Z
			currentPhysicsState.move = MoveOnGroundStates::JumpAttack;
			isJumpAttack = true;
		}
		if (currentPhysicsState.jump == JumpOnAirStates::Fall && currentPhysicsState.move == MoveOnGroundStates::JumpAttack)
		{
			currentPhysicsState.move = MoveOnGroundStates::Idle;
			isAttack = false;
			isJumpAttack = false;
		}
	}

}

void CFireMario::EndAnimation()
{
	CMario::EndAnimation();
	if (currentState.compare(MARIO_STATE_ATTACK) == 0)
	{
		isAttack = false; 
		isJumpAttack = false;
		if (animations.find(lastState) == animations.end() || lastState == currentState) // Không kiếm được last state trong animation, đồng nghĩa với việc last state chưa được khởi tạo, còn nếu đc khởi tạo rồi thì mình set state theo cái state trước đó
		{
			lastState = MARIO_STATE_IDLE;
		}
		SetState(lastState); 
	}
}

void CFireMario::OnKeyDown(int KeyCode)
{
	CMario::OnKeyDown(KeyCode);
	if (isAttack == true)
	{

		auto currentFireBall = fireBalls.Init();
		auto normal = physiscBody->GetNormal();

		if (currentFireBall != NULL)
		{
			auto firePhyBody = currentFireBall->GetPhysiscBody();

			auto posMario = transform.position + relativePositionOnScreen;
			posMario.x += SUPER_MARIO_BBOX.x * 0.5f * normal.x;
			currentFireBall->SetPosition(posMario);
			firePhyBody->SetVelocity(D3DXVECTOR2(FIRE_BALL_SPEED * normal.x, 0));
		}
	}
}

void CFireMario::AddObjectToScene(LPScene scene)
{
	this->fireBalls.AddPoolToScene(scene);
}

CFireMario::~CFireMario()
{
	CGameObject::~CGameObject();

}
