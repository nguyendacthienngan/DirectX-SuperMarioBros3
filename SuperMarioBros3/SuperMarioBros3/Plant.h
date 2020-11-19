#pragma once
#include "Enemy.h"
class CPlant :
    public CEnemy
{
protected:
	bool canDartOut, isIdle;
	float maxHeight;
	DWORD timeStopDartOut, timeStartIdle;
	DWORD timeToStopDartOut, timeToIdle;
public:
	CPlant();
	void Update(DWORD dt, CCamera* cam) override;
};

