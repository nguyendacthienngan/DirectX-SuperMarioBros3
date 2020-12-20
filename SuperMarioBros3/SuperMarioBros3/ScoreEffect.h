#pragma once
#include "EffectObject.h"
#include "Font.h"
class CScoreEffect : public CEffectObject
{
	int currentScore;
	int scoreState;
public:
	CScoreEffect();
	void LoadAnimation() override;

	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha = 255) override;
};