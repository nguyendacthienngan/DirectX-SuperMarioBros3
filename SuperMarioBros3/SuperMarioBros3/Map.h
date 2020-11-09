#pragma once

#ifndef MAP_H
#define MAP_H
#include "Sprite.h"
#include "Camera.h"
#include "GameObject.h"
#include <string>
#include <map>
#include "TileMap.h"

class CGameObject;
typedef CGameObject* LPGameObject;

class CTileMap;

class CMap
{
private:
	CTileMap* tileMap;
	std::vector<LPGameObject> listGameObjects;
public:
	CMap(std::string filePath, std::string fileMap);
	~CMap();
	void LoadTilemap(std::string filePath, std::string fileMap);
	
	void Update(CCamera* camera, DWORD dt);
	void Render(CCamera* camera);

	std::vector<LPGameObject> GetListGameObjects();
};

#endif
