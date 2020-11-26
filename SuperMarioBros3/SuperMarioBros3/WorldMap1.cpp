#include "WorldMap1.h"
#include "Game.h"
#include "Const.h"
#include "SceneManager.h"
CWorldMap1::CWorldMap1()
{
	this->id = "World-Map-1";
	this->filePath = CGame::GetInstance()->GetFilePathByCategory(CATEGORY_SCENE, SC_WOLRD_1);
}

void CWorldMap1::Load()
{
	CScene::Load();
	CSceneManager::GetInstance()->GetUICamera()->SetDisableBlackTexture(true);
}

void CWorldMap1::Unload()
{
	CScene::Unload();
	mario = NULL;
}

CWorldMap1::~CWorldMap1()
{
}
