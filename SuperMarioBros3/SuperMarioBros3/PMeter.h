#pragma once
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
	int pMeterState;
	int feverState;
	bool isRaccoonMario;
	bool canfly;
	int previousFeverState, previousPMeterState;
	bool isDecreaseRapidly;
	//int maxArrowPositionIsCharging;
public:
	CPMeter(D3DXVECTOR2 pos);
	void Update();
	void Render();

	void SetPMeterCounting(float pMeterCounting);
	float GetPMeterCounting();

	void SetFeverState(int feverstate);
	int GetFeverState();

	void SetIsRaccoonMario(bool isRaccoonMario);

	void SetCanFly(bool canfly);
};

