#pragma once
#include "Mario.h"
class CRacoonMario : public CMario
{
private:
	bool isSetToPointCenter;
	bool isAttackContinious;
	DWORD timeToFly, timeToFloat;
	DWORD timeToKeyFlyDown, timeToKeyFloatDown;
	DWORD lastFlyTime; // time để tính từ lúc canFly cho đến khi hết timeToFly
	DWORD lastKeyFlyDown, lastFloatTime, lastKeyFloatDown;
	bool moreFlyPower, moreFloatPower;
	bool flyDown;
	bool canFloat, isFloat; // Giành cho việc ngay khi bay xong có thẩy quẫy đuôi để rơi chậm
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

