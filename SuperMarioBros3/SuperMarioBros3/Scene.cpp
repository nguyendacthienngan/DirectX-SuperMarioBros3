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

//CScene::CScene(std::string filePath)
//{
//	this->filePath = filePath;
//}

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
	for (TiXmlElement* scene = root->FirstChildElement(); scene != NULL; scene = scene->NextSiblingElement())
	{
		string name = scene->Attribute("name");
		if (name.compare("Map") == 0)
		{
			DebugOut(L"[INFO] Load map \n");
			string sourceMap = scene->Attribute("source");
			this->map = new CMap(sourceMap); // Ham nay tu load map
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
			player->SetPosition(startPosition);
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
					//DebugOut(L"[INFO] Camera position (x,y) : %f, %f", camera->GetPositionCam().x, camera->GetPositionCam().y);

				}
			}
			if (player != NULL)
			{
				camera->SetGameObject(player);
			}
			
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
