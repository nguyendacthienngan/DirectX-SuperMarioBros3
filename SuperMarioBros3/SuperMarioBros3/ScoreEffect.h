#pragma once
#include "EffectObject.h"
#include "Font.h"
class CScoreEffect : public CEffectObject
{
	CFont* scoreText;
	int currentScore;
	int scoreState;
public:
	CScoreEffect();
	void SetStartPosition(D3DXVECTOR2 pos);

	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha = 255) override;
};