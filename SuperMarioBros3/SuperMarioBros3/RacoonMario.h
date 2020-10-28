#pragma once
#include "Mario.h"
class CRacoonMario : public CMario
{
private:
	bool isSetToPointCenter;
public:
	CRacoonMario();
	void Init() override;
	void LoadAnimation() override;
	void EndAnimation() override;
	void Update(DWORD dt, CCamera* cam) override;
	~CRacoonMario();
};

