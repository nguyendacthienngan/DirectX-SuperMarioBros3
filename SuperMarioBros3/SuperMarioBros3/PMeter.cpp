#include "PMeter.h"
#include "Ultis.h"
#include "UICameraConst.h"
CPMeter::CPMeter(D3DXVECTOR2 pos)
{
	this->pos = pos;
	auto startPos = pos;
	for (int i = 0; i < 6; i++)
	{
		CArrowItemIcon* arrowItem = new CArrowItemIcon();
		arrowItem->SetPosition(startPos);
		arrowItemIcons.push_back(arrowItem);
		startPos.x += arrowItem->GetSize().x;
	}
	startPos.x += SPACE_BETWEEN_ARROW_AND_PICON;
	pIcon = new CPIcon();
	pIcon->SetPosition(startPos);
	pMeterCounting = 0.0f;
	pMeterState = -1;
	feverState = -1;
}

void CPMeter::Update()
{
	if (pMeterCounting <= 0.1f)
		pMeterState = -1;

	if (InRange(pMeterCounting, 0.1f, 1.0f) == true)
		pMeterState = 0;

	if (InRange(pMeterCounting, 1.0f, 2.0f) == true)
		pMeterState = 1;

	if (InRange(pMeterCounting, 2.0f, 3.0f) == true)
		pMeterState = 2;

	if (InRange(pMeterCounting, 3.0f, 4.0f) == true)
		pMeterState = 3;

	if (InRange(pMeterCounting, 4.0f, 5.0f) == true)
		pMeterState = 4;

	if (InRange(pMeterCounting, 5.0f, 6.0f) == true)
		pMeterState = 5;

	if (pMeterState != -1)
	{
		if (feverState == 1 || feverState == 2)
			arrowItemIcons[pMeterState]->SetCharged(true);
		if (feverState == 3)
			arrowItemIcons[pMeterState]->SetCharged(false);
	}
	pIcon->SetCharged((feverState == 2));
}

void CPMeter::Render()
{
	for (int i = 0; i < 6; i++)
		arrowItemIcons[i]->Render();
	pIcon->Render();
}

void CPMeter::SetPMeterCounting(float pMeterCounting)
{
	this->pMeterCounting = pMeterCounting;
}

float CPMeter::GetPMeterCounting()
{
	return pMeterCounting;
}

void CPMeter::SetFeverState(int feverState)
{
	this->feverState = feverState;
}

int CPMeter::GetFeverState()
{
	return feverState;
}
