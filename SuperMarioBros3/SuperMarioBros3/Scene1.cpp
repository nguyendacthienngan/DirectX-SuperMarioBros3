#include "Scene1.h"
#include "Ultis.h"

#include "MapConst.h"
#include "Const.h"
#include "MarioConst.h"
CScene1::CScene1()
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


	DebugOut(L"[INFO] Load Map ... \n");
	DebugOut(L"[INFO] Map Path: ");
	OutputDebugString(ToLPCWSTR(MAP_PATH + MAP_WORLD1_1_FILENAME));
	DebugOut(L"\n");

	map = new CMap(MAP_PATH + MAP_WORLD1_1_FILENAME);
	auto gameObjects = map->GetListGameObjects();
	if (gameObjects.size() == 0)
		DebugOut(L"Cannot load game objects from map \n");
	int count = 0;
	for (auto gO : gameObjects)
	{
		count++;
		AddObject(gO);
		
	}
	DebugOut(L"Load %d objects \n", count);

	DebugOut(L"[INFO] Init Cam ... \n");
	camera = new CCamera(SCREEN_WIDTH, SCREEN_HEIGHT);
	//camera->SetPositionCam(D3DXVECTOR2(48.0f, 720.0f)); // Hard code: Tọa độ theo hệ quy chiếu world
	
	camera->SetPositionCam(D3DXVECTOR2(MARIO_START_X, MARIO_START_Y - SCREEN_HEIGHT/2));
	camera->SetBoundary(pos.x, map->GetWidth() - 1000);
	camera->SetGameObject(mario);

	backgroundColor = D3DCOLOR_XRGB(156, 252, 240);
	DebugOut(L"[INFO] Loaded Scene \n");
}


CScene1::~CScene1()
{
	delete mario;
}
