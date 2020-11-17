#pragma once
#include "Enemy.h"
class CVenus :  public CEnemy
{
public:
	CVenus();
	void Init();
	virtual void LoadAnimation();
	void Update(DWORD dt, CCamera* cam) override;
};

