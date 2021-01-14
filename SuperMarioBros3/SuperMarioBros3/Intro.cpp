#include "Intro.h"
#include "Ultis.h"

#include "MapConst.h"
#include "Const.h"
#include "MarioConst.h"
#include "Game.h"
#include "SceneManager.h"
CIntro::CIntro()
{
	this->id = SC_INTRO;
	this->filePath = CGame::GetInstance()->GetFilePathByCategory(CATEGORY_SCENE, SC_INTRO);
}

void CIntro::Load()
{
	CScene::Load();
	CSceneManager::GetInstance()->GetUICamera()->SetDisableBlackTexture(true);
	gameObjects.push_back(new CMenu());
}