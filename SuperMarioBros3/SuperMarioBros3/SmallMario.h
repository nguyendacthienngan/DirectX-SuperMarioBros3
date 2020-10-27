#pragma once
#include "Mario.h"
class CSmallMario : public CMario
{
public:
	CSmallMario();
	void Init() override;
	void LoadAnimation() override;
	~CSmallMario();
};

