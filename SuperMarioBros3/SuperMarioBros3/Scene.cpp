#include "Scene.h"
#include "Ultis.h"
#include "tinyxml.h"
#include "Mario.h"
#include "Game.h"
#include <string>
using namespace std;

CScene::CScene()
{
}

CScene::CScene(std::string filePath)
{
	this->filePath = filePath;
}

void CScene::Load()
{
	// Đọc file Scenes/world1-1.xml
	TiXmlDocument sceneFile(filePath.c_str());
	if (!sceneFile.LoadFile())
	{
		DebugOut(L"[ERROR] Cannot load file \n");
		return;
	}
	TiXmlElement* root = sceneFile.RootElement();
	CMario* player = new CMario();
	for (TiXmlElement* scene = root->FirstChildElement(); scene != NULL; scene->NextSiblingElement())
	{
		string name = scene->Attribute("name");
		if (name.compare("Map") == 0)
		{
			string sourceMap = scene->Attribute("source");
			CMap* map = new CMap(sourceMap); // Ham nay tu load map
			auto mapSolidBoxs = map->GetListGameObjects();
			for (auto obj : mapSolidBoxs)
			{
				AddObject(obj);
			}
			for (TiXmlElement* color = scene->FirstChildElement(); color != NULL ; color->NextSiblingElement() )
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
			D3DXVECTOR2 startPosition;
			scene->QueryFloatAttribute("pos_x", &startPosition.x);
			scene->QueryFloatAttribute("pos_y", &startPosition.y);
			player->SetPosition(startPosition);
			AddObject(player);
		}
		else if (name.compare("Camera") == 0)
		{
			int screenWidth = CGame::GetInstance()->GetScreenWidth();
			int screenHeight = CGame::GetInstance()->GetScreenHeight();
			CCamera* camera = new CCamera(screenWidth, screenHeight);
			int start;
			scene->QueryIntAttribute("start", &start);

			for (TiXmlElement* boundary = scene->FirstChildElement(); boundary != NULL; boundary->NextSiblingElement())
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
					
				}
			}
			// co can check lai NULL hay k?
			D3DXVECTOR2 posCam = D3DXVECTOR2(player->GetPosition().x, screenHeight / 2);
			camera->SetPositionCam(posCam);
			camera->SetGameObject(player);
		}
	}
}

void CScene::Unload()
{
	for (auto object : gameObjects)
		delete object;
}

void CScene::Update(DWORD dt)
{

	//DebugOut(L"[INFO] Updating Scene \n");
	if (gameObjects.size() == 0) return;
	for (auto obj : gameObjects)
	{
		if (obj->IsEnabled() == false) continue;
		obj->PhysicsUpdate(&gameObjects); 
		obj->Update(dt, camera);
		obj->LateUpdate();
		obj->AnimationUpdate();
		// Set transform animation ở đây ??
	}

	if (camera != NULL)
		map->Update(camera, dt);
}

void CScene::Render()
{
	//DebugOut(L"[INFO] Draw Map ... \n");
	//map->Draw(camera); // Load dựa trên camera nhưng chưa set vị trí camera đúng được

	map->Render(camera);

	//DebugOut(L"[INFO] Draw Map Successfully \n");

	//DebugOut(L"[INFO] Rendering Scene \n");
	if (gameObjects.size() == 0) return;

	for (auto obj : gameObjects)
	{
		// TO DO: Enable objects
		if (obj->IsEnabled() == false) continue;
		obj->Render(camera);
	}
}

void CScene::AddObject(LPGameObject gameObject)
{
	if (gameObject != NULL)
		gameObjects.push_back(gameObject);
	//DebugOut(L"[INFO] Objects' length: %d \n", gameObjects.size());
}

void CScene::RemoveObject(LPGameObject gameObject)
{
	auto remObj = find(gameObjects.begin(), gameObjects.end(), gameObject);
	if (remObj != gameObjects.end())
	{
		gameObjects.erase(remObj);
	}
}

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
	delete map; // đúng hay chưa?
	delete camera;
	for (auto gO : gameObjects)
		delete gO;
	gameObjects.clear();
}
