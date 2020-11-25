﻿#pragma once
#include "ArrowItemIcon.h"
#include "PIcon.h"
#include <vector>
class CPMeter
{
private:
	// vector bao gồm 6 arrow
	std::vector<CArrowItemIcon*> arrowItemIcons;
	// p button
	CPIcon* pIcon;
	D3DXVECTOR2 pos;
	float pMeterCounting;
	int feverState;
public:
	CPMeter(D3DXVECTOR2 pos);
	void Update();
	void Render();
	void SetPMeterCounting(float pMeterCounting);
	float GetPMeterCounting();
};

