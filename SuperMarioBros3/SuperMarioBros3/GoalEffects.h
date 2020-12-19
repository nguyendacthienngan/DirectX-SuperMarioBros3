#pragma once
#include "EffectObject.h"
class CGoalEffects :  public CEffectObject
{

public:
	CGoalEffects();
	void LoadAnimation() override;
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
};

