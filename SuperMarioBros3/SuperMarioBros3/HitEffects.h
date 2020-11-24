#pragma once
#include "EffectObject.h"
#define TIME_HEAD_SHOT_ANIMATION	300

class CHitEffects : public CEffectObject
{
	DWORD startHitTime;
public:
	CHitEffects();
	void LoadAnimation();
	void EndAnimation() override;

	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha = 255) override;
	void SetStartHitTime(DWORD t);

};

