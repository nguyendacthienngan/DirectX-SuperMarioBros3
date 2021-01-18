#pragma once

#ifndef MAP_H
#define MAP_H

#include <string>
#include <map>

#include "Sprite.h"
#include "Camera.h"
#include "GameObject.h"
#include "TileMap.h"
#include "Scene.h"

class CGameObject;
typedef CGameObject* LPGameObject;

class CTileMap;
class CScene; 

class CMap
{
private:
	CTileMap* tileMap;
	std::vector<LPGameObject> listGameObjects;
public:
	CMap(std::string filePath, std::string fileMap, CGameObject* player, CScene* scene);
	~CMap();
	void LoadTilemap(std::string filePath, std::string fileMap, CGameObject* player, CScene* scene);
	
	void Update(CCamera* camera, DWORD dt);
	void Render(CCamera* camera, bool isRenderForeground);

	std::vector<LPGameObject> GetListGameObjects();
	CTileMap* GetTileMap();
};

#endif
