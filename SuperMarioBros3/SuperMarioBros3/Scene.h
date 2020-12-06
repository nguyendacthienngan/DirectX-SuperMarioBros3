#pragma once

#ifndef SCENE_H
#define SCENE_H

#include "GameObject.h"

#include "Map.h"
#include "Camera.h"
class CScene;
typedef CScene* LPScene;

class CGameObject;
typedef CGameObject* LPGameObject;

class CCamera;
class CMap;

class CScene
{
protected:
	std::vector<LPGameObject> gameObjects;
	std::string id;
	D3DCOLOR backgroundColor;
	CMap *map;
	CCamera *camera;
	std::string filePath;
	bool loaded;
	std::vector<LPGameObject> destroyObjects, initObjects, updateObjects;
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

	bool IsLoaded();
	virtual ~CScene();
};
#endif
