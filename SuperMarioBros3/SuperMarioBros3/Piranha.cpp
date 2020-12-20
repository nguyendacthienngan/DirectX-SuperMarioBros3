#include "Piranha.h"
#include "AnimationManager.h"
#include "PiranhaConst.h"
#include "Ultis.h"
#include "Game.h"

CPiranha::CPiranha()
{
	CPiranha::LoadAnimation();
	CPiranha::Init();
}

void CPiranha::Init()
{
	canDartOut = false;
	isIdle = false;
	timeStartIdle = 0;
	timeStopDartOut = 0;
	isEnabled = true;
	SetState(PIRANHA_STATE_ATTACK);

	enemyTag = EnemyTag::Piranha;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(PIRANHA_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Piranha");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
}

void CPiranha::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(PIRANHA_STATE_ATTACK, animationManager->Clone("ani-green-piranha-plant-attack"));
}

void CPiranha::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	//CPlant::Update(dt, cam, uiCam);
	

	if (timeStopDartOut != 0)
		timeStopDartOut += CGame::GetInstance()->GetDeltaTime() * CGame::GetTimeScale();
	if (timeStartIdle != 0)
		timeStartIdle += CGame::GetInstance()->GetDeltaTime() * CGame::GetTimeScale();

	if (canDartOut == true)
	{
		// Trong trạng thái ngoi lên
		timeStopDartOut = 0;
		// Khi chưa đạt max height thì cung cấp vy, không thì trả lại vy
		if (abs(startPosition.y) - abs(transform.position.y) > maxHeight || abs(transform.position.y) - abs(startPosition.y) > maxHeight)
		{
			if (timeStartIdle == 0) // Đứng yên
			{
				timeStartIdle = CGame::GetInstance()->GetDeltaTime() * CGame::GetTimeScale();
				physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
				isIdle = true;
			}
			if (timeStartIdle > timeToIdle) // Đi xuống
			{
				physiscBody->SetVelocity(D3DXVECTOR2(0.0f, PIRANHA_PUSH_FORCE));
				isIdle = false;
			}
		}
		else if (transform.position.y >= startPosition.y)
		{
			canDartOut = false;
		}
	}
	if (timeStopDartOut > timeToStopDartOut && timeStopDartOut != 0)
	{
		// Mới vô đợi 0.8s để được ngoi lên
		canDartOut = true;
		physiscBody->SetVelocity(D3DXVECTOR2(0.0f, -PIRANHA_PUSH_FORCE));
		return;
	}
	if (canDartOut == false && timeStopDartOut == 0)
	{
		// Ở dưới cái pipe
		timeStopDartOut = CGame::GetInstance()->GetDeltaTime() * CGame::GetTimeScale();
		physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
		timeStartIdle = 0;
		
	}
	if (isIdle == false && timeStartIdle != 0)
	{
		physiscBody->SetVelocity(D3DXVECTOR2(0.0f, PIRANHA_PUSH_FORCE));
		transform.position.y += PIRANHA_PUSH_FORCE * dt;
	}
}