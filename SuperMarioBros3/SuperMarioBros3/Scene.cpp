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

	//DebugOut(L"[INFO] Updating Scene \n");
	if (gameObjects.size() == 0) return;
	for (auto obj : gameObjects)
	{
		if (obj->IsEnabled() == false) continue;
		obj->PhysicsUpdate(&gameObjects); 
		obj->Update(dt, camera);
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
		if (obj->GetTag() == "player") // so sanh chuoi nen dung ham compare ,k nen ==, oh okay
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
