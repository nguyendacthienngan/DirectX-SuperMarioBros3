#include "Scene1.h"
#include "Ultis.h"

#include "MapConst.h"
#include "Const.h"

#define MARIO_START_X 0
#define MARIO_START_Y 489


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
	camera = new CCamera(SCREEN_WIDTH, SCREEN_HEIGHT);
	camera->SetPositionCam(D3DXVECTOR2(0, 1248 - (600 - 48))); // Hard code

	//camera = new CCamera(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	//camera->SetPositionCam(D3DXVECTOR2(0 , 1248 - (300 - 48))); // Hard code

	float boundaryRight = (float)(map->GetWidth() - camera->GetWidthCam());
	camera->SetBoundary(0.0f, boundaryRight);

	backgroundColor = D3DCOLOR_XRGB(156, 252, 240);
	DebugOut(L"[INFO] Loaded Scene \n");
}


void CScene1::Render()
{

	DebugOut(L"[INFO] Draw Map ... \n");
	//map->Draw(); // Còn bị lag là do chưa chỉnh lại camera. Ta load hết nguyên map và vẽ hết nguyên map sẽ rất nặng
	//map->Draw(camera); // Load dựa trên camera nhưng chưa set vị trí camera đúng được

	map->Render(camera);

	DebugOut(L"[INFO] Draw Map Successfully \n");
	CScene::Render();
}

CScene1::~CScene1()
{
	delete mario;
}
