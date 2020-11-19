#pragma once
#include "Plant.h"
#include "ObjectPool.h"
class CVenus :  public CPlant
{
	int countFireBalls;
	D3DXVECTOR2 vectorShootFireBall;
	CObjectPool fireBalls;
public:
	CVenus();
	void Init();
	virtual void LoadAnimation();
	void Update(DWORD dt, CCamera* cam) override;
	void Render(CCamera* cam, int alpha = 255);
	CObjectPool GetObjectPool();
};

