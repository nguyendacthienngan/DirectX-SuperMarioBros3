#pragma once
#include "Enemy.h"
class CPiranha :  public CEnemy
{
protected:
	bool canDartOut;
	DWORD timeStopDartOut;
public:
	CPiranha();
	void Init();
	virtual void LoadAnimation();
	void Update(DWORD dt, CCamera* cam) override;
};

