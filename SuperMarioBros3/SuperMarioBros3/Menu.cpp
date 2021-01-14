#include "Menu.h"
#include "Ultis.h"
#include "SpriteManager.h"
#include <dinput.h>
#include "WorldMap1.h"
#include "SceneManager.h"

CMenu::CMenu()
{
	tag = GameObjectTags::Menu;
	selection1 = new CFont();
	auto pos = D3DXVECTOR2(250,440);
	selection1->SetCurrentText("1 PLAYER GAME");
	selection1->SetPosition(pos);

	pos.y += 50;
	selection2 = new CFont();
	selection2->SetCurrentText("2 PLAYER GAME");
	selection2->SetPosition(pos);

	auto spriteManager = CSpriteManager::GetInstance();
	arrow = spriteManager->Get("spr-menu-arrow-0");

	isEnabled = true;

	direction.insert(make_pair("up", D3DXVECTOR2(200, 440)));
	direction.insert(make_pair("down", D3DXVECTOR2(200, 490)));
	currentDirection = direction.at("up");
}

void CMenu::Render(CCamera* cam, int alpha)
{
	selection1->Render();
	selection2->Render();
	arrow->Draw(currentDirection, D3DXVECTOR2(1.0f, 1.0f), 0.0f);
}

void CMenu::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
		case DIK_DOWN:
		{
			currentDirection = direction.at("down");
			break;
		}
		case DIK_UP:
		{
			currentDirection = direction.at("up");
			break;
		}
		case DIK_Q:
		{
			if (currentDirection == direction.at("up"))
				currentDirection = direction.at("down");
			else
				currentDirection = direction.at("up");
			break;
		}
		case DIK_W:
		{
			CWorldMap1* world1 = new CWorldMap1();
			CSceneManager::GetInstance()->SwitchScene(world1);
			break;
		}
	}
}

void CMenu::OnKeyUp(int KeyCode)
{
}
