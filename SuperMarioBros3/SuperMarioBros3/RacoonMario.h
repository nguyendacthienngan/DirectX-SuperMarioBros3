#pragma once
#include "Mario.h"
class CRacoonMario : public CMario
{
private:
	bool isSetToPointCenter;
	bool isAttackContinious;
	DWORD timeToFly;
	DWORD timeToKeyFlyDown;
	DWORD lastFlyTime; // time để tính từ lúc canFly cho đến khi hết timeToFly
	DWORD lastKeyFlyDown;
	bool isFly;
	bool moreFlyPower;
public:
	CRacoonMario();
	void Init() override;
	void LoadAnimation() override;
	void EndAnimation() override;
	void Update(DWORD dt, CCamera* cam) override;

	void OnKeyDown(int KeyCode) override;
	void OnKeyUp(int KeyCode) override;

	~CRacoonMario();
};

