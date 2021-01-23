#include "Scene.h"
#include "Ultis.h"
#include "tinyxml.h"
#include "Mario.h"
#include "Game.h"
#include "MarioController.h"
#include "SceneConst.h"
#include "Koopa.h"
#include "CVenus.h"
#include "ObjectPool.h"
#include "UICamera.h"

#include <string>
#include "SceneManager.h"
#include "MarioMap.h"
#include "Card.h"
#include "QuestionBlock.h"
#include "Brick.h"

using namespace std;

CScene::CScene()
{
	camera = NULL;
	loaded = true;
	cardState = 0;
	marioController = NULL;
	spaceParitioning = true;
	canRenderForeground = true;
}

void CScene::SetRenderForeground(bool canRender)
{
	canRenderForeground = canRender;
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
		if (name.compare("Player") == 0)
		{
			DebugOut(L"[INFO] Load player \n");
			D3DXVECTOR2 startPosition;
			scene->QueryFloatAttribute("pos_x", &startPosition.x);
			scene->QueryFloatAttribute("pos_y", &startPosition.y);
			player = new CMarioController();
			player->AddStateObjectsToScene(this);
			player->SetPosition(startPosition);
			player->GetCurrentStateObject()->SetPosition(startPosition);
			AddObject(player);
			marioController = player;
		}
		if (name.compare("Map") == 0)
		{
			DebugOut(L"[INFO] Load map \n");
			string sourceMap;
			string fileMap;

			if (spaceParitioning == true)
			{
				sourceMap = scene->Attribute("gridSource");
				fileMap = scene->Attribute("gridFile");
			}
			else
			{
				sourceMap = scene->Attribute("source");
				fileMap = scene->Attribute("fileName");
			}
			this->map = NULL;
			this->map = new CMap(sourceMap, fileMap, player, this); // Ham nay tu load map
			auto tilemap = map->GetTileMap();

			bricks = tilemap->GetBricks();
			coins = tilemap->GetCoins();
			poolBricks = tilemap->GetPoolBricks();
			poolCoins = tilemap->GetPoolCoins();
			poolBricks->AddPoolToScene(this);
			poolCoins->AddPoolToScene(this);
			card = tilemap->GetCard();

			if (spaceParitioning == true)
			{
				this->grid = map->GetTileMap()->GetGrid();
			}
			else
			{
				auto mapObjs = map->GetListGameObjects();
				for (auto obj : mapObjs)
				{
					AddObject(obj);
				}
			}

			DebugOut(L"[INFO] Load background color \n");
			for (TiXmlElement* color = scene->FirstChildElement(); color != NULL; color = color->NextSiblingElement())
			{
				int R, G, B;
				color->QueryIntAttribute("R", &R);
				color->QueryIntAttribute("G", &G);
				color->QueryIntAttribute("B", &B);
				backgroundColor = D3DCOLOR_XRGB(R, G, B);
			}
		}
		else if (name.compare("Camera") == 0)
		{
			DebugOut(L"[INFO] Load camera \n");
			int viewportWidth, viewportHeight, start;
			scene->QueryIntAttribute("start", &start);
			scene->QueryIntAttribute("width", &viewportWidth);
			scene->QueryIntAttribute("height", &viewportHeight);

			this->camera = new CCamera(viewportWidth, viewportHeight);

			for (TiXmlElement* boundary = scene->FirstChildElement(); boundary != NULL; boundary = boundary->NextSiblingElement())
			{
				int id, disX, disY, autoX;
				RectF bound;
				D3DXVECTOR2 pos;
				boundary->QueryIntAttribute("id", &id);

				boundary->QueryFloatAttribute("pos_x", &pos.x);
				boundary->QueryFloatAttribute("pos_y", &pos.y);
				boundary->QueryFloatAttribute("left", &bound.left);
				boundary->QueryFloatAttribute("top", &bound.top);
				boundary->QueryFloatAttribute("right", &bound.right);
				boundary->QueryFloatAttribute("bottom", &bound.bottom);

				boundary->QueryIntAttribute("disX", &disX);
				boundary->QueryIntAttribute("disY", &disY);
				boundary->QueryIntAttribute("autoX", &autoX);

				camera->AddCameraProperties(id, pos, bound, disX, disY);
				if (start == id)
				{
					camera->SetCurrentBoundary(bound);
					camera->SetPositionCam(pos);
					camera->SetDisablePosX(disX);
					camera->SetDisablePosY(disY);
					camera->SetAutoX(autoX);
				}
			}
			if (player != NULL)
			{
				camera->SetGameObject(player->GetCurrentStateObject());
			}
		}
		if (name.compare("Player-Map") == 0)
		{
			DebugOut(L"[INFO] Load player in map\n");
			D3DXVECTOR2 startPosition;
			scene->QueryFloatAttribute("pos_x", &startPosition.x);
			scene->QueryFloatAttribute("pos_y", &startPosition.y);

			CMarioMap* marioMap = new CMarioMap();
			marioMap->SetPosition(startPosition);
			marioMap->SetStartPosition(startPosition);
			AddObject(marioMap);
			marioController = marioMap;
		}
	}
	if (marioController != NULL)
		keyboardTargetObjects.push_back(marioController);
	loaded = true;
}

void CScene::Unload()
{
	loaded = false;
	if (spaceParitioning == false)
	{
		if (gameObjects.size() > 0)
		{
			for (int i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i]->SetDestroy(true);
			}
		}
	}
}

void CScene::DestroyObject()
{
	if (loaded == false)
	{
		if (spaceParitioning == true)
		{
			marioController = NULL;
			keyboardTargetObjects.clear();

			if (globalObjects.size() > 0)
			{
				/*for (auto gO : globalObjects)
				{
					if (gO->GetTag() == GameObjectTags::PlayerController)
						continue;
					delete gO;
					gO = NULL;
				}*/
				globalObjects.clear();
			}
			//delete grid;
			grid = NULL;
		}
		else
		{
			//for (auto gO : gameObjects) // IMPORTANT for player problem
			//{
			//	if (gO->IsDestroyed() == true)
			//	{
			//		if (gO->GetTag() == GameObjectTags::PlayerController)
			//			continue;
			//		RemoveObject(gO);
			//		delete gO;
			//		gO = NULL;
			//	}
			//}
			gameObjects.clear();
		}
		delete map;
		map = NULL;
		camera = NULL;
	}
	if (spaceParitioning == false) /// IMPORTANT for brick problem?
	{
		if (destroyObjects.size() > 0)
		{
			/*for (auto gO : destroyObjects)
			{
				RemoveObject(gO);
				delete gO;
				gO = NULL;
			}*/
			destroyObjects.clear();
		}
	}
}

void CScene::Update(DWORD dt)
{
	if (loaded == false)
		return;
	if (card != NULL && CGame::GetTimeScale() != 0)
	{
		auto cardGO = static_cast<CCard*>(card);
		if (cardGO->IsTouched() == false)
		{
			cardState++;
			if (cardState > 10)
				cardState = 0;
			cardGO->SetCardState(cardState);
		}
		else
		{
			cardGO = NULL;
			card = NULL;
		}
	}
	auto uiCam = CSceneManager::GetInstance()->GetUICamera();
	if (updateObjects.size() == 0) return;
	D3DXVECTOR2 oldPosition;
	for (auto obj : updateObjects)
	{
		if (obj->IsIgnoreTimeScale() == false && CGame::GetTimeScale() == 0)
			continue;
		if (obj->IsEnabled() == false)
			continue;
		if (uiCam != NULL)
			obj->Update(dt, camera, uiCam);
		else
			obj->Update(dt, camera, NULL);
		oldPosition = obj->GetPosition();
		obj->PhysicsUpdate(&updateObjects);
		if (obj->IsInGrid() == true)
			grid->Move(oldPosition, obj);
	}
	if (camera != NULL)
		map->Update(camera, dt);
}

void CScene::Render()
{
	if (loaded == false)
		return;
	map->Render(camera, false);
	if (updateObjects.size() == 0) return;

	for (auto obj : updateObjects)
	{
		if (obj->IsEnabled() == false)								continue;

		obj->Render(camera);
		if (obj->GetCollisionBox()->size() != 0)
			obj->GetCollisionBox()->at(0)->Render(camera, CollisionBox_Render_Distance);
	}
	if (canRenderForeground == true)
		map->Render(camera, true);
}

void CScene::FindUpdateObjects()
{
	updateObjects.clear();
	if (spaceParitioning == true)
	{
		auto activeCells = grid->FindActiveCells(camera);
		int count = 0;
		for (auto activeCell : activeCells)
		{
			for (auto gameObject : activeCell->GetListGameObject())
			{
				count++;
				updateObjects.push_back(gameObject);
			}
		}
		if (globalObjects.size() > 0)
		{
			for (auto obj : globalObjects)
			{
				if (camera != NULL
					&& camera->CheckObjectInCamera(obj) == false)			continue;
				updateObjects.push_back(obj);
			}
		}
	}
	else
	{
		if (gameObjects.size() == 0) return;
		for (auto obj : gameObjects)
		{
			if (camera != NULL
				&& camera->CheckObjectInCamera(obj) == false)			continue;
			updateObjects.push_back(obj);
		}
	}
}

void CScene::AddObject(LPGameObject gameObject)
{
	if (gameObject == NULL)
		return;
	if (spaceParitioning == true)
	{
		if (CheckGlobalObject(gameObject->GetTag()) == false)
		{
			if (grid == NULL)
				return;
			if (gameObject->IsInGrid() == false)
			{
				grid->Insert(gameObject);
				gameObject->SetInGrid(true);
			}
		}
		else
			globalObjects.push_back(gameObject);
	}
	else
	{
		auto gameObj = find(gameObjects.begin(), gameObjects.end(), gameObject);
		if (gameObj == gameObjects.end())
		{
			gameObjects.push_back(gameObject);
		}
	}
}

void CScene::RemoveObject(LPGameObject gameObject)
{
	if (gameObject == NULL)
		return;
	if (spaceParitioning == true)
	{
		grid->Remove(gameObject);
	}
	else
	{
		auto gameObj = find(gameObjects.begin(), gameObjects.end(), gameObject);
		if (gameObj != gameObjects.end())
		{
			gameObjects.erase(gameObj);
		}
	}
}

void CScene::AddGlobalObject(LPGameObject gameObject)
{
	if (gameObject == NULL)
		return;
	globalObjects.push_back(gameObject);
}

CGameObject* CScene::GetMarioController()
{
	return marioController;
}

std::vector<LPGameObject> CScene::GetBricks()
{
	return bricks;
}

std::vector<LPGameObject> CScene::GetCoins()
{
	return coins;
}

CObjectPool* CScene::GetPoolBricks()
{
	return poolBricks;
}

CObjectPool* CScene::GetPoolCoins()
{
	return poolCoins;
}

void CScene::RemoveBrick(CGameObject* gameObject)
{
	auto gameObj = find(bricks.begin(), bricks.end(), gameObject);
	if (gameObj != bricks.end())
	{
		bricks.erase(gameObj);
	}
}

void CScene::RemoveCoin(CGameObject* gO)
{
	auto gameObj = find(coins.begin(), coins.end(), gO);
	if (gameObj != coins.end())
	{
		coins.erase(gameObj);
	}
}

void CScene::AddBrick(CGameObject* gO)
{
	if (gO != NULL)
		bricks.push_back(gO);
}

void CScene::AddCoin(CGameObject* gO)
{
	if (gO != NULL)
		coins.push_back(gO);
}

void CScene::AddDestroyObject(CGameObject* gO)
{
	if (gO != NULL)
		destroyObjects.push_back(gO);
}

bool CScene::SwitchBlockStateOnToOff()
{
	return switchBlockOffToOn;
}

void CScene::SwitchBlockStateOnToOff(bool state)
{
	this->switchBlockOffToOn = state;
}

void CScene::SetCamera(int id)
{
	auto camProps = camera->GetCameraProperties(id);
	if (CameraPropertieSet::IsEmpty(camProps) == false)
	{
		camera->SetCurrentBoundary(camProps.boundarySet);
		camera->SetPositionCam(camProps.camPosition);
		camera->SetDisablePosX(camProps.disableX);
		camera->SetDisablePosY(camProps.disableY);
	}
}

bool CScene::IsLoaded()
{
	return loaded;
}

bool CScene::IsSpacePartitioning()
{
	return spaceParitioning;
}

bool CScene::CheckGlobalObject(GameObjectTags tag)
{
	if (tag == GameObjectTags::Solid || tag == GameObjectTags::GhostPlatform || tag == GameObjectTags::Pipe || tag == GameObjectTags::MovingPlatform)
		return true;
	if (tag == GameObjectTags::Player || tag == GameObjectTags::SmallPlayer || tag == GameObjectTags::PlayerController)
		return true;
	if (tag == GameObjectTags::RaccoonTail)
		return true;
	if (tag == GameObjectTags::VenusFireBall || tag == GameObjectTags::Boomerang)
		return true;
	return false;
}

void CScene::AddKeyboardTargetObject(CGameObject* gameObject)
{
	keyboardTargetObjects.push_back(gameObject);
}

std::vector<LPGameObject>  CScene::GetKeyboardTargetObject()
{
	return keyboardTargetObjects;
}
CGrid* CScene::GetGrid()
{
	return grid;
}

CScene::~CScene()
{
}
