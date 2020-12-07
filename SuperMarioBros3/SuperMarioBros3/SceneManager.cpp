#include "SceneManager.h"
#include "Ultis.h"
#include "Game.h"
#include "Const.h"
LPSceneManager CSceneManager::instance = NULL;

CSceneManager::CSceneManager()
{
}

LPSceneManager CSceneManager::GetInstance()
{
	if (instance == NULL) instance = new CSceneManager();
	return instance;
}

void CSceneManager::Init()
{
	auto filePath = CGame::GetInstance()->GetFilePathByCategory(CATEGORY_SCENE, SC_UI_CAMERA);
	TiXmlDocument sceneFile(filePath.c_str());
	if (!sceneFile.LoadFile())
	{
		DebugOut(L"[ERROR] Cannot load file \n");
		return;
	}
	TiXmlElement* root = sceneFile.RootElement();
	for (TiXmlElement* ui = root->FirstChildElement(); ui != NULL; ui = ui->NextSiblingElement())
	{
		string name = ui->Attribute("name");
		if (name.compare("UICamera") == 0)
		{
			DebugOut(L"[INFO] Load UI camera \n");
			int screenWidth = CGame::GetInstance()->GetScreenWidth();
			int screenHeight = CGame::GetInstance()->GetScreenHeight();

			D3DXVECTOR2 pos, posHUD;
			ui->QueryFloatAttribute("pos_x", &pos.x);
			ui->QueryFloatAttribute("pos_y", &pos.y);

			TiXmlElement* uiCam = ui->FirstChildElement();
			std::string nameUICam = uiCam->Attribute("name");

			if (nameUICam.compare("HUD") == 0)
			{
				uiCam->QueryFloatAttribute("pos_x", &posHUD.x);
				uiCam->QueryFloatAttribute("pos_y", &posHUD.y);
				this->uiCamera = new CUICamera(screenWidth, screenHeight, posHUD);
				this->uiCamera->SetPositionCam(pos);
			}
		}
	}
	
}


//Load Resource
void CSceneManager::Load(LPScene scene)
{
	scene->Load(); // Scene Manager nó tự load
	loadedScenes.insert(make_pair(scene->GetSceneId(), scene));
	activeSceneId = scene->GetSceneId();
}

void CSceneManager::Unload(std::string sceneId)
{
	if (loadedScenes.find(sceneId) != loadedScenes.end())
		loadedScenes.at(sceneId)->Unload();
}

LPScene CSceneManager::GetScene(std::string id)
{
	return loadedScenes[id];
}

CUICamera* CSceneManager::GetUICamera()
{
	return uiCamera;
}

std::string CSceneManager::GetActiveSceneId()
{
	return activeSceneId;
}

LPScene CSceneManager::GetActiveScene()
{
	//OutputDebugString(ToLPCWSTR("[INFO] Active Scene: " + activeSceneId + "\n"));
	if (activeSceneId == "") return nullptr;
	if (loadedScenes.find(activeSceneId) != loadedScenes.end())
	{
		return loadedScenes.at(activeSceneId);
	}
	return nullptr;
}

void CSceneManager::LoadRequestScene()
{
	if (requestedLoadScene.size() > 0)
	{
		DebugOut(L"Hello \n");
		auto scene = requestedLoadScene.at(0);
		if (scene != NULL)
		{
			requestedLoadScene.erase(requestedLoadScene.begin());
			Load(scene);
		}
	}
}

void CSceneManager::UnloadRequestScene()
{
	if (requestedUnloadScene.size() > 0)
	{
		auto scene = requestedUnloadScene.at(0);
		if (scene != NULL)
		{
			requestedUnloadScene.erase(requestedUnloadScene.begin());
			Unload(scene->GetSceneId());
		}
	}
}

void CSceneManager::SwitchScene(LPScene scene)
{
	auto activeScene = GetActiveScene();
	if (activeScene == NULL)
		return;
	requestedUnloadScene.push_back(activeScene);
	//Unload(activeSceneId);
	requestedLoadScene.push_back(scene);
}

CSceneManager::~CSceneManager()
{
	for (auto s : loadedScenes)
	{
		delete s.second;
	}
	loadedScenes.clear();
}
