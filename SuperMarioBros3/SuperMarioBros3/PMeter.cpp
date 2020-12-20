#include "PMeter.h"
#include "Ultis.h"
#include "UICameraConst.h"
#include "PMeterConst.h"
#include "Game.h"
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
	feverState = -2;
	previousFeverState = -2;
	isRaccoonMario = false;
	previousPMeterState = -1;
	isDecreaseRapidly = false;
}

void CPMeter::Update()
{
	// PMeterState:
	// -2 : Mario bthg ban đầu
	// -1: Raccoon Mario ban đầu
	// 0: Bắt đầu 
	
	if (pMeterCounting <= 0.1f)
		pMeterState = -2;
	
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

	if (pMeterState >= 0)
	{
		// Nếu là mario bình thường không phải là raccoon thì là 1,2
		// Nếu là raccoon mario thì 4,5
		if (previousPMeterState <= pMeterCounting)
		{
			if (isRaccoonMario == false && (feverState == 1 || feverState == 2))
				arrowItemIcons[pMeterState]->SetCharged(true);
			if (isRaccoonMario == true && (feverState == 4 || feverState == 5))
				arrowItemIcons[pMeterState]->SetCharged(true);
		}
		else // Giảm p-meter
		{
			if (pMeterState < 5)
			{
				if (isRaccoonMario == false && (feverState == 0 || feverState == 1))
					arrowItemIcons[pMeterState + 1]->SetCharged(false);
				if (isRaccoonMario == true && (feverState == 4 || feverState == 5)) // Cho nay con loi
				arrowItemIcons[pMeterState + 1]->SetCharged(false);
			}
		}

		// 3 hoặc 6 thì disable
		if (feverState == 3 || feverState == 6)
			arrowItemIcons[pMeterState]->SetCharged(false);
	}
	// Nếu feverState = 2 hoặc canFly = true thì pIcon sáng đèn
	pIcon->SetCharged((feverState == 2 || canfly == true));

	// Nếu xuống 0 thì hạ từ cái hiện tại xuống pMeterState = -1 hoặc -2 làm sao cho mượt nhất
	if (pMeterCounting == 0)
	{
		if (previousPMeterState >= 0)
		{
			if (previousPMeterState == 6)
				arrowItemIcons[previousPMeterState -1]->SetCharged(false);
			else 
				arrowItemIcons[previousPMeterState]->SetCharged(false);
			previousPMeterState--;
		}
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
	if (this->pMeterCounting == pMeterCounting)
		return;
	previousPMeterState = this->pMeterCounting;
	this->pMeterCounting = pMeterCounting;
}

float CPMeter::GetPMeterCounting()
{
	return pMeterCounting;
}

void CPMeter::SetFeverState(int feverState)
{
	if (this->feverState == feverState)
		return;
	previousFeverState = this->feverState;
	this->feverState = feverState;
}

int CPMeter::GetFeverState()
{
	return feverState;
}

void CPMeter::SetIsRaccoonMario(bool isRaccoonMario)
{
	this->isRaccoonMario = isRaccoonMario;
}

void CPMeter::SetCanFly(bool canfly)
{
	this->canfly = canfly;
}
