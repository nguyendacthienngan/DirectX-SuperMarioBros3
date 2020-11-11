#include "Scene.h"
#include "Ultis.h"
#include "tinyxml.h"
#include "Mario.h"
#include "Game.h"
#include "MarioController.h"
#include <string>
#include "Koopa.h"
using namespace std;

CScene::CScene()
{
}

void CScene::Load()
{
	TiXmlDocument sceneFile(filePath.c_str());
	if (!sceneFile.LoadFile())
	{
		DebugOut(L"[ERROR] Cannot load file \n");
		return;
	}
	TiXmlElement* root = sceneFile.RootElement();
	CMarioController* player = NULL;
	for (TiXmlElement* scene = root->FirstChildElement(); scene != NULL; scene = scene->NextSiblingElement())
	{
		string name = scene->Attribute("name");
		if (name.compare("Map") == 0)
		{
			DebugOut(L"[INFO] Load map \n");
			string sourceMap = scene->Attribute("source");
			string fileMap = scene->Attribute("fileName");
			this->map = new CMap(sourceMap, fileMap); // Ham nay tu load map
			auto mapSolidBoxs = map->GetListGameObjects();
			for (auto obj : mapSolidBoxs)
			{
				AddObject(obj);
			}
			DebugOut(L"[INFO] Load background color \n");
			for (TiXmlElement* color = scene->FirstChildElement(); color != NULL ; color = color->NextSiblingElement() )
			{
				int R, G, B;
				color->QueryIntAttribute("R", &R);
				color->QueryIntAttribute("G", &G);
				color->QueryIntAttribute("B", &B);
				backgroundColor = D3DCOLOR_XRGB(R, G, B);
			}
		}
		else if (name.compare("Player") == 0)
		{
			DebugOut(L"[INFO] Load player \n");
			D3DXVECTOR2 startPosition;
			scene->QueryFloatAttribute("pos_x", &startPosition.x);
			scene->QueryFloatAttribute("pos_y", &startPosition.y);

			player = new CMarioController();
			player->AddStateObjectsToScene(this);
			player->GetCurrentStateObject()->SetPosition(startPosition);
			AddObject(player);
		}
		else if (name.compare("Camera") == 0)
		{
			DebugOut(L"[INFO] Load camera \n");
			int screenWidth = CGame::GetInstance()->GetScreenWidth();
			int screenHeight = CGame::GetInstance()->GetScreenHeight();
			this->camera = new CCamera(screenWidth, screenHeight);
			int start;
			scene->QueryIntAttribute("start", &start);

			for (TiXmlElement* boundary = scene->FirstChildElement(); boundary != NULL; boundary = boundary->NextSiblingElement())
			{
				int id;
				float pos_x, pos_y, left, top, right, bottom;
				boundary->QueryIntAttribute("id", &id);
				if (start == id)
				{
					boundary->QueryFloatAttribute("pos_x", &pos_x);
					boundary->QueryFloatAttribute("pos_y", &pos_y);
					boundary->QueryFloatAttribute("left", &left);
					boundary->QueryFloatAttribute("top", &top);
					boundary->QueryFloatAttribute("right", &right);
					boundary->QueryFloatAttribute("bottom", &bottom);

					camera->SetBoundary(left, right, top, bottom);
					camera->SetPositionCam(D3DXVECTOR2(pos_x, pos_y));
				}
			}
			if (player != NULL)
			{
				camera->SetGameObject(player->GetCurrentStateObject());
			}
			
		}
	}
}

void CScene::Unload()
{
	for (int i = 0; i < gameObjects.size()-2 ; i++)
	{
		RemoveObject(gameObjects[i]);
		delete gameObjects[i];
		gameObjects[i] = NULL;
	}
	map = NULL;
	camera = NULL;
	gameObjects.clear();
}

void CScene::Update(DWORD dt)
{
	if (gameObjects.size() == 0) return;
	for (auto obj : gameObjects)
	{
		if (obj->IsEnabled() == false) continue;
		obj->Update(dt, camera);
		obj->PhysicsUpdate(&gameObjects); 
	}

	if (camera != NULL)
		map->Update(camera, dt);
}

void CScene::Render()
{
	map->Render(camera);
	if (gameObjects.size() == 0) return;

	for (auto obj : gameObjects)
	{
		if (obj->IsEnabled() == false) continue;
		obj->Render(camera);
		if (obj->GetCollisionBox()->size() != 0)
			obj->GetCollisionBox()->at(0)->Render(camera, -24);
	}
}

void CScene::AddObject(LPGameObject gameObject)
{
	if (gameObject != NULL)
		gameObjects.push_back(gameObject);
}

void CScene::RemoveObject(LPGameObject gameObject)
{
	auto gameObj = find(gameObjects.begin(), gameObjects.end(), gameObject);
	if (gameObj != gameObjects.end())
	{
		gameObjects.erase(gameObj);
	}
}

void CScene::SetObjectPosition(D3DXVECTOR2 distance)
{
	for (auto obj : gameObjects)
	{
		if (obj->GetTag() == GameObjectTags::Solid)
		{
			auto pos = obj->GetCollisionBox()->at(0)->GetPosition();
			obj->GetCollisionBox()->at(0)->SetPosition(pos + distance);
		}
	}
}

//void CScene::SetMarioController(CMarioController* mC)
//{
//	marioController = mC;
//}
//
//CMarioController* CScene::GetMarioController()
//{
//	return marioController;
//}

std::vector<LPGameObject> CScene::GetObjects()
{
	return gameObjects;
}

LPGameObject CScene::GetPlayer()
{
	LPGameObject player = NULL;
	for (auto obj : gameObjects)
		if (obj->GetTag() == GameObjectTags::Player) 
			player = obj;
	return player;
}

CScene::~CScene()
{
}
