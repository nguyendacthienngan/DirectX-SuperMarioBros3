#include "Menu.h"
#include "Ultis.h"

CMenu::CMenu()
{
	selection1 = new CFont();
	auto pos = D3DXVECTOR2(250,440);
	selection1->SetCurrentText("1 PLAYER GAME");
	selection1->SetPosition(pos);

	pos.y += 50;
	selection2 = new CFont();
	selection2->SetCurrentText("2 PLAYER GAME");
	selection2->SetPosition(pos);
}

void CMenu::Update()
{
}

void CMenu::Render()
{
	selection1->Render();
	selection2->Render();
}
