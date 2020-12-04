#pragma once
#include "Gift.h"
class CLeafEffect :  public CGift
{
private:
	bool isBounce;
	bool isFall;
	DWORD startFallTime;
public:
	CLeafEffect();
	void LoadAnimation();
	virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam);
	void Render(CCamera* cam, int alpha = 255) override;
	void StartEffect();
};

