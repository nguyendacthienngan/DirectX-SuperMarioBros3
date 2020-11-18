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
	timeStartIdle = 0;
	canDartOut = false;
	maxHeight = PIRANHA_HEIGHT;
	timeToStopDartOut = DART_OUT_TIME;
	timeToIdle = DART_OUT_TIME;
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
		if (abs(startPosition.y) - abs(transform.position.y) > maxHeight || abs(transform.position.y) - abs(startPosition.y)  > maxHeight)
		{
			if (timeStartIdle == 0)
			{
				timeStartIdle = GetTickCount64();
				physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));

			}
			if (GetTickCount64() - timeStartIdle > timeToIdle)
				physiscBody->SetVelocity(D3DXVECTOR2(0.0f, PIRANHA_PUSH_FORCE));
		}
		else if (transform.position.y >= startPosition.y)
		{
			canDartOut = false;
		}
		return;
	}
	if (GetTickCount64() - timeStopDartOut > timeToStopDartOut && timeStopDartOut != 0)
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
		timeStartIdle = 0;
		physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
		return;
	}
}