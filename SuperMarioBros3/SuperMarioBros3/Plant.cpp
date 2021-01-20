#include "Plant.h"
#include "PiranhaConst.h"
#include "SceneManager.h"
#include "Game.h"

CPlant::CPlant()
{
	timeStopDartOut = GetTickCount64();
	timeStartIdle = 0;
	canDartOut = false;
	maxHeight = PIRANHA_HEIGHT;
	timeToStopDartOut =  TIME_TO_IDLE;
	timeToIdle = DART_OUT_TIME;
	isIdle = false;
}

void CPlant::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
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

void CPlant::OnDie()
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	activeScene->AddObject(hitFX);
	hitFX->SetStartPosition(this->transform.position);
	hitFX->SetStartHitTime(GetTickCount64());
	hitFX->Enable(true);
	activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), hitFX);
	this->isEnabled = false;
	this->physiscBody->SetDynamic(false);
	this->collisionBoxs->at(0)->SetEnable(false);
}
