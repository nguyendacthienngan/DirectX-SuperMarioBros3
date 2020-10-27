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
		if (animations.find(lastState) == animations.end()) // Không kiếm được last state trong animation, đồng nghĩa với việc last state chưa được khởi tạo, còn nếu đc khởi tạo rồi thì mình set state theo cái state trước đó
			lastState = MARIO_STATE_IDLE;
		SetState(lastState); // bị lỗi, vì khi attack xong nó dừng ở frame cuối của animation và giữ state là last state => Attack => K chuyển sang trạng thái IDLE khi đang đứng yên (bắn lửa xong), phải di chuyển r mới thay đổi state đc
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
		}
	}
	else
	{
		// Bị lỗi, ngay khi ấn xong đáng ra Fire Mario phải chuyển từ Attack về Idle (Khi end animation) thì lúc này vô đc else. Nhưng lúc mình ấn nó chưa xong animation?
		countFireBall = 0;
		DebugOut(L"Count fireball: %d \n", countFireBall);
	}
		
}

CFireMario::~CFireMario()
{
	for (auto fireBall : listFireBalls)
	{
		delete fireBall;
	}
	//delete currentFireBall;
}
