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
	if (camera != NULL)
		map->Update(camera, dt);

	DebugOut(L"[INFO] Updating Scene \n");
	if (gameObjects.size() == 0) return;
	for (auto obj : gameObjects)
	{
		obj->Update(dt);
		obj->AnimationUpdate();
		// Set transform animation ở đây ??
	}

	// Đáng ra ta phải gọi update camera theo Mario, cơ mà để chạy được trc tiên thì mình thử dựa theo input đã
	//map->Update(camera, dt);
}

void CScene::Render()
{
	DebugOut(L"[INFO] Draw Map ... \n");
	//map->Draw(); // Còn bị lag là do chưa chỉnh lại camera. Ta load hết nguyên map và vẽ hết nguyên map sẽ rất nặng
	//map->Draw(camera); // Load dựa trên camera nhưng chưa set vị trí camera đúng được

	map->Render(camera);

	DebugOut(L"[INFO] Draw Map Successfully \n");

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
	delete map; // đúng hay chưa?
	delete camera;
	for (auto gO : gameObjects)
		delete gO;
	gameObjects.clear();
}
