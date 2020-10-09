#include "Scene1.h"
#include "Ultis.h"

#define MARIO_START_X 10.0f
#define MARIO_START_Y 130.0f

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
	DebugOut(L"[INFO] Loaded Scene \n");
}


CScene1::~CScene1()
{
}
