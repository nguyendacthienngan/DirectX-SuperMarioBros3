#pragma once
#include "ParaKoopa.h"
class CRedParaKoopa :
    public CParaKoopa
{
	float boundaryTop;
	float boundaryBottom;
public:
	CRedParaKoopa();
	void LoadAnimation() override;
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha) override;
	void SetBoundary(float bTop, float bBottom);
};

