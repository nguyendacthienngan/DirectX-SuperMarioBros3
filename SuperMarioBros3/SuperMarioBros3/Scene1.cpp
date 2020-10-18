#include "Scene1.h"
#include "Ultis.h"

#include "MapConst.h"
#include "Const.h"

#define MARIO_START_X 70
#define MARIO_START_Y 1204 // Tọa độ theo hệ quy chiếu world

//#define MARIO_START_X 20
//#define MARIO_START_Y 484
 // Lúc này là tọa độ theo hệ quy chiếu camera

//#define MARIO_START_X 48
//#define MARIO_START_Y 720
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

	camera->SetPositionCam(D3DXVECTOR2(pos.x, pos.y - SCREEN_HEIGHT/2));
	camera->SetBoundary(pos.x, map->GetWidth() - 1000);

	backgroundColor = D3DCOLOR_XRGB(156, 252, 240);
	DebugOut(L"[INFO] Loaded Scene \n");
}


CScene1::~CScene1()
{
	delete mario;
}
