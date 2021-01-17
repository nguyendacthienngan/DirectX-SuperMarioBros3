#pragma once

#ifndef SCENE_H
#define SCENE_H

//#include "GameObject.h"

#include "Map.h"
#include "Camera.h"
#include "ObjectPool.h"
#include "Grid.h"
class CScene;
typedef CScene* LPScene;

class CGameObject;
typedef CGameObject* LPGameObject;

class CCamera;
class CMap;

class CObjectPool;

class CGrid;

class CScene
{
protected:
	std::vector<LPGameObject> gameObjects;
	std::string id;
	D3DCOLOR backgroundColor;

	CGrid* grid;
	CMap *map;
	CCamera *camera;
	std::string filePath;
	bool loaded;
	std::vector<LPGameObject> destroyObjects, initObjects, updateObjects;

	std::vector<LPGameObject> bricks;
	std::vector<LPGameObject> coins;
	CObjectPool* poolBricks;
	CObjectPool* poolCoins;
	bool switchBlockOffToOn;

	CGameObject* card;
	int cardState;
public:
	CScene();
	virtual void Load();
	virtual void Unload();
	virtual void DestroyObject();
	virtual void Update(DWORD dt); // dt để xác định t va chạm 
	virtual void Render();
	virtual void FindUpdateObjects();
	std::string GetSceneId() { return this->id; }
	D3DCOLOR GetBackgroundColor() { return backgroundColor; }

	void SetCamera(int id);
	CCamera* GetCamera() { return camera; }
	CMap* GetMap() { return map; }
	std::vector<LPGameObject> GetObjects();
	LPGameObject GetPlayer();
	void AddObject(LPGameObject gameObject);
	void RemoveObject(LPGameObject gameObject);

	void SetObjectPosition(D3DXVECTOR2 distance);

	std::vector<CGameObject*> GetBricks();
	std::vector<CGameObject*> GetCoins();
	CObjectPool* GetPoolBricks();
	CObjectPool* GetPoolCoins();

	void RemoveBrick(CGameObject* gO);
	void RemoveCoin(CGameObject* gO);
	void AddBrick(CGameObject* gO);
	void AddCoin(CGameObject* gO);

	void AddDestroyObject(CGameObject* gO);
	bool SwitchBlockStateOnToOff();
	void SwitchBlockStateOnToOff(bool state);
	bool IsLoaded();
	virtual ~CScene();
};
#endif
