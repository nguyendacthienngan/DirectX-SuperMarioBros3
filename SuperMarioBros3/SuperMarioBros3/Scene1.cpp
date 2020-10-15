#include "Scene1.h"
#include "Ultis.h"

#include "MapConst.h"
#include "Const.h"

//#define MARIO_START_X 0
//#define MARIO_START_Y 500
#define MARIO_START_X 0
#define MARIO_START_Y 484
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

	DebugOut(L"[INFO] Init Cam ... \n");
	camera = new CCamera(SCREEN_WIDTH, SCREEN_HEIGHT, mario);
	camera->SetPositionCam(D3DXVECTOR2(48.0f, 720.0f)); // Hard code: Tọa độ theo hệ quy chiếu world
	//camera->SetPositionCam(D3DXVECTOR2(48.0f , 703.0f )); // Hard code: Tọa độ theo hệ quy chiếu world


	//camera->SetPositionCam(D3DXVECTOR2(pos.x - SCREEN_WIDTH/2, pos.y - SCREEN_HEIGHT/2));
	//camera->SetPositionCam(D3DXVECTOR2(48, 718)); 
	//camera->SetPositionCam(D3DXVECTOR2(0, 1248 - (600 - 48)));
	//camera->SetPositionCam(D3DXVECTOR2(0 , 1248 - (300 - 48)));

	float boundaryRight = (float)(map->GetWidth() - camera->GetWidthCam());
	camera->SetBoundary(0.0f, boundaryRight);

	backgroundColor = D3DCOLOR_XRGB(156, 252, 240);
	DebugOut(L"[INFO] Loaded Scene \n");
}


CScene1::~CScene1()
{
	delete mario;
}
