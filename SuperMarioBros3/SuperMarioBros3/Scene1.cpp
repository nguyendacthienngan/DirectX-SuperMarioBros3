#include "Scene1.h"
#include "Ultis.h"

#include "MapConst.h"
#include "Const.h"
#include "MarioConst.h"
#include "Game.h"

#include "FireBall.h"
CScene1::CScene1()
{
	this->id = "PlayScene";
	this->filePath = CGame::GetInstance()->GetFilePathByCategory(CATEGORY_SCENE, SC_WOLRD_1_1);
}


void CScene1::Load()
{
	CScene::Load();
	DebugOut(L"[INFO] Loaded Scene \n");
}

void CScene1::Unload()
{
	CScene::Unload();
	mario = NULL;
}

CScene1::~CScene1()
{
}
