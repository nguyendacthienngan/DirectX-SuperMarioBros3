#pragma once
#include "Mario.h"
class CRacoonMario : public CMario
{
public:
	CRacoonMario();
	void Init() override;
	void LoadAnimation() override;
	void EndAnimation() override;
};

