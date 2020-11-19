#pragma once
#include "Plant.h"
class CPiranha :  public CPlant
{
public:
	CPiranha();
	void Init();
	virtual void LoadAnimation();
	void Update(DWORD dt, CCamera* cam) override;
};

