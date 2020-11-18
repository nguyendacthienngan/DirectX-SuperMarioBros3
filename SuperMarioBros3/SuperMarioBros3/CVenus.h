#pragma once
#include "Piranha.h"
class CVenus :  public CPiranha
{
	CGameObject* target;
	int countFireBalls;
	D3DXVECTOR2 vectorShootFireBall;
public:
	CVenus();
	void Init();
	virtual void LoadAnimation();
	void Update(DWORD dt, CCamera* cam) override;
};

