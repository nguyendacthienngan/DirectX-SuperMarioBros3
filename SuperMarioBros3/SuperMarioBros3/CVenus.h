#pragma once
#include "Piranha.h"
class CVenus :  public CPiranha
{
public:
	CVenus();
	void Init();
	virtual void LoadAnimation();
	void Update(DWORD dt, CCamera* cam) override;
};

