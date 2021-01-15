#pragma once
#include "ParaKoopa.h"
class CRedParaKoopa :
    public CParaKoopa
{

public:
	CRedParaKoopa();
	void LoadAnimation() override;
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
};

