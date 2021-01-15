#pragma once
#include "ParaKoopa.h"
class CGreenParaKoopa :
    public CParaKoopa
{
private:
	float jumpStartPosition;
	bool isJump;
public:
	CGreenParaKoopa();
	void LoadAnimation() override;
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;

};

