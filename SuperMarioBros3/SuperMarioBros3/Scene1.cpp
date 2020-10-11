#include "Scene1.h"
#include "Ultis.h"

#include "MapConst.h"

#define MARIO_START_X 100.0f
#define MARIO_START_Y 50.0f

CScene1::CScene1()
{
}

void CScene1::OnKeyDown(int KeyCode)
{
}

void CScene1::OnKeyUp(int KeyCode)
{
}

void CScene1::Load()
{
	this->id = "PlayScene";
	mario = new CMario();
	D3DXVECTOR2 pos; 
	pos.x = MARIO_START_X;
	pos.y = MARIO_START_Y;
	mario->SetPosition(pos);
	this->AddObject(mario);

	

	backgroundColor = D3DCOLOR_XRGB(156, 252, 240);
	DebugOut(L"[INFO] Loaded Scene \n");
}


void CScene1::Render()
{
	DebugOut(L"[INFO] Load Map ... \n");
	map = new CMap(MAP_PATH + MAP_WORLD1_1_FILENAME);
	map->Draw();
	DebugOut(L"[INFO] Load Map Successfully \n");
	CScene::Render();
}

CScene1::~CScene1()
{
}
