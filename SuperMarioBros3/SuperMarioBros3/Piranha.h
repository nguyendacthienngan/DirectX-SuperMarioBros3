#pragma once
#include "Enemy.h"
class CPiranha :  public CEnemy
{
protected:
	bool isDartOut; // ngoi ra khỏi ống cống (pipe)
	bool canDartOut;
	DWORD timeStopDartOut;
public:
	CPiranha();
	void Init();
	virtual void LoadAnimation();
	void Update(DWORD dt, CCamera* cam) override;
};

