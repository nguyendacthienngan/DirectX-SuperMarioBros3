#pragma once
#include "Mario.h"
class CRacoonMario : public CMario
{
private:
	bool isSetToPointCenter;
	bool isAttackContinious;
	DWORD timeToFly;
	DWORD lastFlyTime;
	bool isFly;
public:
	CRacoonMario();
	void Init() override;
	void LoadAnimation() override;
	void EndAnimation() override;
	void Update(DWORD dt, CCamera* cam) override;
	void OnKeyUp(int KeyCode) override;
	~CRacoonMario();
};

