#pragma once

#ifndef SCENE_H
#define SCENE_H

#include "GameObject.h"

#include "Map.h"
#include "Camera.h"

class CScene;
typedef CScene* LPScene;

class CScene
{
protected:
	std::vector<LPGameObject> gameObjects;
	std::string id;
	//LPCWSTR filePath;
	D3DCOLOR backgroundColor;
	CMap *map;
	CCamera *camera;
	std::string filePath;
public:
	CScene();
	//CScene(std::string filePath);

	virtual void Load();
	virtual void Unload();

	virtual void Update(DWORD dt); // dt để xác định t va chạm 
	virtual void Render();

	std::string GetSceneId() { return this->id; }
	D3DCOLOR GetBackgroundColor() { return backgroundColor; }

	CCamera* GetCamera() { return camera; }
	std::vector<LPGameObject> GetObjects();
	LPGameObject GetPlayer();
	void AddObject(LPGameObject gameObject);
	void RemoveObject(LPGameObject gameObject);


	virtual ~CScene();
};
#endif
