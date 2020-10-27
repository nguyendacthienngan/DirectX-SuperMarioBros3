#pragma once
#include "Mario.h"
class CSuperMario: public CMario
{
public:
	CSuperMario();
	void Init() override;
	void LoadAnimation() override;
	~CSuperMario();
};

