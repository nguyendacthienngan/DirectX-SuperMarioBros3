#pragma once
#include "Enemy.h"
class CPiranha :  public CEnemy
{
protected:
	bool canDartOut;
	float maxHeight;
	DWORD timeStopDartOut, timeStartIdle;
	DWORD timeToStopDartOut, timeToIdle;
public:
	CPiranha();
	void Init();
	virtual void LoadAnimation();
	void Update(DWORD dt, CCamera* cam) override;
};

