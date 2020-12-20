#pragma once
#include "Enemy.h"
class CPlant :
    public CEnemy
{
protected:
	bool canDartOut, isIdle;
	float maxHeight;
	int timeStopDartOut, timeStartIdle;
	int timeToStopDartOut, timeToIdle;
public:
	CPlant();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void OnDie() override;
};

