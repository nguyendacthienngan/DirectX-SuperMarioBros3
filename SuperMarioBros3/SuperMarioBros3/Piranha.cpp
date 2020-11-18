#include "Piranha.h"
#include "AnimationManager.h"
#include "PiranhaConst.h"
#include "Ultis.h"

CPiranha::CPiranha()
{
	LoadAnimation();
	Init();
}

void CPiranha::Init()
{
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

	timeStopDartOut = GetTickCount64();
	timeToIdle = 0;
	canDartOut = false;
}

void CPiranha::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(PIRANHA_STATE_ATTACK, animationManager->Clone("ani-green-piranha-plant-attack"));
}

void CPiranha::Update(DWORD dt, CCamera* cam)
{
	auto velocity = physiscBody->GetVelocity();
	
	if (canDartOut == true)
	{
		// Trong trạng thái ngoi lên
		timeStopDartOut = 0;
		// Khi chưa đạt max height thì cung cấp vy, không thì trả lại vy
		if (abs(startPosition.y) - abs(transform.position.y) > PIRANHA_HEIGHT || abs(transform.position.y) - abs(startPosition.y)  > PIRANHA_HEIGHT)
		{
			if (timeToIdle == 0)
			{
				timeToIdle = GetTickCount64();
				physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));

			}
			if (GetTickCount64() - timeToIdle > DART_OUT_TIME)
				physiscBody->SetVelocity(D3DXVECTOR2(0.0f, PIRANHA_PUSH_FORCE));
		}
		else if (transform.position.y >= startPosition.y)
		{
			canDartOut = false;
		}
		return;
	}
	if (GetTickCount64() - timeStopDartOut > DART_OUT_TIME && timeStopDartOut != 0)
	{
		// Mới vô đợi 0.8s để được ngoi lên
		canDartOut = true;
		physiscBody->SetVelocity(D3DXVECTOR2(0.0f, -PIRANHA_PUSH_FORCE));
		return;
	}
	if (canDartOut == false && timeStopDartOut == 0)
	{
		// Ở dưới cái pipe
		timeStopDartOut = GetTickCount64();
		timeToIdle = 0;
		physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
		return;
	}
}