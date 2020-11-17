#include "Map.h"

#include <d3dx9.h>
#include <d3d9.h>

#include "TextureManager.h"
#include "MapConst.h"
#include "Ultis.h"
#include "SolidBox.h"
#include "MarioConst.h"
#include "Game.h"
#include "Const.h"
using namespace std;

CMap::CMap(std::string filePath, std::string fileMap)
{
	LoadTilemap(filePath, fileMap);
}

CMap::~CMap()
{
	DebugOut(L"[INFO] Deleting map \n");
	delete tileMap;
}

void CMap::LoadTilemap(std::string filePath, std::string fileMap)
{
	tileMap = new CTileMap();
	tileMap = tileMap->LoadMap(filePath, fileMap, listGameObjects);
}

void CMap::Update(CCamera* camera, DWORD dt)
{
	if (camera == NULL)
		return;
	camera->Update();
}

void CMap::Render(CCamera* camera, bool isRenderForeground)
{
	if (camera == NULL)
	{
		DebugOut(L"[ERROR] Camera is NULL \n");
		return;
	}

	tileMap->Render(camera, isRenderForeground);
}

std::vector<LPGameObject> CMap::GetListGameObjects()
{
	return listGameObjects;
}
