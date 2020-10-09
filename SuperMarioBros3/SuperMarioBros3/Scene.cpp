#include "Scene.h"
#include "Ultis.h"
using namespace std;

CScene::CScene()
{
}

void CScene::Unload()
{
	for (auto object : gameObjects)
		delete object;
}

void CScene::Update(DWORD dt)
{
	DebugOut(L"[INFO] Updating Scene \n");
	if (gameObjects.size() == 0) return;
	for (auto obj : gameObjects)
	{
		obj->Update(dt);
		obj->AnimationUpdate();
	}
}

void CScene::Render()
{
	DebugOut(L"[INFO] Rendering Scene \n");
	if (gameObjects.size() == 0) return;

	for (auto obj : gameObjects)
	{
		// TO DO: Enable objects

		obj->Render();
	}
}

void CScene::AddObject(LPGameObject gameObject)
{
	if (gameObject != NULL)
		gameObjects.push_back(gameObject);
	DebugOut(L"[INFO] Objects' length: %d \n", gameObjects.size());
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

CScene::~CScene()
{
}
