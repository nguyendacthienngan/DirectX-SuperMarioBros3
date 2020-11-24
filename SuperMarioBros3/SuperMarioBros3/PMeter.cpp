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
}

void CPMeter::Render()
{
	for (int i = 0; i < 6; i++)
		arrowItemIcons[i]->Render();
	pIcon->Render();
}
