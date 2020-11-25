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
	feverState = 0;
}

void CPMeter::Update()
{
	if (InRange(pMeterCounting, 0.1f, 1.0f) == true)
		arrowItemIcons[0]->SetCharged(true);
	//else if ()
	//	arrowItemIcons[0]->SetCharged(false);

	if (InRange(pMeterCounting, 1.0f, 2.0f) == true)
		arrowItemIcons[1]->SetCharged(true);
	if (InRange(pMeterCounting, 2.0f, 3.0f) == true)
		arrowItemIcons[2]->SetCharged(true);
	if (InRange(pMeterCounting, 3.0f, 4.0f) == true)
		arrowItemIcons[3]->SetCharged(true);
	if (InRange(pMeterCounting, 4.0f, 5.0f) == true)
		arrowItemIcons[4]->SetCharged(true);
	if (InRange(pMeterCounting, 5.0f, 6.0f) == true)
	{
		arrowItemIcons[5]->SetCharged(true);
		pIcon->SetCharged(true);
	}
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
