#pragma once
#include "Piranha.h"
class CVenus :  public CPiranha
{
	int countFireBalls;
	D3DXVECTOR2 vectorShootFireBall;
public:
	CVenus();
	void Init();
	virtual void LoadAnimation();
	void Update(DWORD dt, CCamera* cam) override;
	void Render(CCamera* cam, int alpha = 255);

};

