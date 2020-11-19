#include "Plant.h"
#include "PiranhaConst.h"

CPlant::CPlant()
{
	timeStopDartOut = GetTickCount64();
	timeStartIdle = 0;
	canDartOut = false;
	maxHeight = PIRANHA_HEIGHT;
	timeToStopDartOut = DART_OUT_TIME;
	timeToIdle = DART_OUT_TIME;
	isIdle = false;
}

void CPlant::Update(DWORD dt, CCamera* cam)
{
	auto velocity = physiscBody->GetVelocity();

	if (canDartOut == true)
	{
		// Trong trạng thái ngoi lên
		timeStopDartOut = 0;
		// Khi chưa đạt max height thì cung cấp vy, không thì trả lại vy
		if (abs(startPosition.y) - abs(transform.position.y) > maxHeight || abs(transform.position.y) - abs(startPosition.y) > maxHeight)
		{
			if (timeStartIdle == 0)
			{
				timeStartIdle = GetTickCount64();
				physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
				isIdle = true;
			}
			if (GetTickCount64() - timeStartIdle > timeToIdle)
			{
				physiscBody->SetVelocity(D3DXVECTOR2(0.0f, PIRANHA_PUSH_FORCE));
				isIdle = false;
			}
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
