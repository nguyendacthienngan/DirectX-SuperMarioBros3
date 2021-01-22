#include "Scene4.h"
#include "Ultis.h"

#include "MapConst.h"
#include "Const.h"
#include "MarioConst.h"
#include "Game.h"
#include "SceneManager.h"
CScene4::CScene4()
{
	this->id = SC_WOLRD_1_4;
	this->filePath = CGame::GetInstance()->GetFilePathByCategory(CATEGORY_SCENE, SC_WOLRD_1_4);
}

void CScene4::Load()
{
	CScene::Load();
	CSceneManager::GetInstance()->GetUICamera()->SetDisableBlackTexture(false);
	auto timer = CSceneManager::GetInstance()->GetUICamera()->GetHUD()->GetTimer();
	timer->StartTimer(); // Mỗi scene khi load đều start timer trừ map selection
}

void CScene4::Unload()
{
	CScene::Unload();
	auto timer = CSceneManager::GetInstance()->GetUICamera()->GetHUD()->GetTimer();
	timer->ResetTimer();
}

void CScene4::Update(DWORD dt)
{
	if (camera->GetPositionCam().x > 6000)
		camera->SetAutoX(false);
	CScene::Update(dt);
}

CScene4::~CScene4()
{
}
