#pragma once
#include "Mario.h"
#include "RaccoonTailBox.h"

class CRaccoonTailBox;

class CRaccoonMario : public CMario
{
private:
	CRaccoonTailBox* raccoonTailBox;

	bool isSetToPointCenter;
	bool isAttackContinious;
	DWORD timeToFly, timeToFloat, timeToAttack;
	DWORD timeToKeyFlyDown, timeToKeyFloatDown;
	DWORD lastFlyTime; // time để tính từ lúc canFly cho đến khi hết timeToFly
	DWORD lastKeyFlyDown, lastFloatTime, lastKeyFloatDown;
	bool moreFlyPower, moreFloatPower;
	bool flyDown;
	bool canFloat, isFloat; // Giành cho việc ngay khi bay xong có thẩy quẫy đuôi để rơi chậm
	bool beginAttackTail;
public:
	CRaccoonMario();
	void Init() override;
	void LoadAnimation() override;
	void EndAnimation() override;
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void AddObjectToScene(LPScene scene);

	void OnKeyDown(int KeyCode) override;
	void OnKeyUp(int KeyCode) override;
	CRaccoonTailBox* GetRaccoonTailBox();
	void SetRaccoonTailBox(CRaccoonTailBox*);

	void ResetValueAttack();
	void ResetValueFly();
	bool IsFly();
	~CRaccoonMario();
};

