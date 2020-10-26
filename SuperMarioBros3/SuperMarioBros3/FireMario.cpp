#include "FireMario.h"
#include "AnimationManager.h"
#include "MarioConst.h"
#include "Misc.h"
#include "MiscConst.h"
#include "SceneManager.h"

CFireMario::CFireMario()
{
	CMario::Init();
	SetTag(GameObjectTags::FireMario);
	CFireBall* fireBall1 = new CFireBall(); 
	fireBall1->SetFireMario(this);
	listFireBalls.push_back(fireBall1);
	CFireBall* fireBall2 = new CFireBall();
	fireBall2->SetFireMario(this);
	listFireBalls.push_back(fireBall2);

	Init();
	lastState = currentState;
	canCrouch = true;
	canAttack = true;
	currentFireBall = NULL;

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
		isAttack = false; // có bị phạm luật hay không?
		if (animations.find(lastState) == animations.end()) // Không kiếm được last state trong animation, đồng nghĩa với việc last state chưa được khởi tạo, còn nếu đc khởi tạo rồi thì mình set state theo cái state trước đó
			lastState = MARIO_STATE_IDLE;
		SetState(lastState);
	}
}

void CFireMario::AddMiscToScene(CScene* scene)
{
	CGameObject::AddMiscToScene(scene);
	for (auto fireBall : listFireBalls)
	{
		scene->AddObject(fireBall);
	}
}

void CFireMario::OnKeyDown(int KeyCode)
{
	CMario::OnKeyDown(KeyCode);
	if (isAttack == true)
	{
		currentFireBall = listFireBalls[0];
		if (currentFireBall == NULL)
			return;
		auto firePhyBody = currentFireBall->GetPhysiscBody();

		currentFireBall->Enable(true);
		auto normal = physiscBody->GetNormal();

		auto posMario = transform.position;
		posMario.x += SUPER_MARIO_BBOX.x *0.5f *normal.x;
		firePhyBody->SetVelocity(D3DXVECTOR2(FIRE_BALL_SPEED * normal.x, 0.0f));

		currentFireBall->SetPosition(transform.position);
	}
	else
	{
		if (currentFireBall != NULL)
		{
			currentFireBall->Enable(false);
		}
	}
}
