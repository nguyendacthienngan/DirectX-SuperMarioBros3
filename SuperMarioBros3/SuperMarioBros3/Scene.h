#pragma once

#ifndef SCENE_H
#define SCENE_H

#include "GameObject.h"

#include "Map.h"

class CScene;
typedef CScene* LPScene;

class CScene
{
protected:
	std::vector<LPGameObject> gameObjects;
	std::string id;
	LPCWSTR filePath;
	D3DCOLOR backgroundColor;
	CMap *map;
public:
	CScene();

	virtual void Load() = 0;
	virtual void Unload();

	virtual void Update(DWORD dt); // dt để xác định t va chạm 
	virtual void Render();

	std::string GetSceneId() { return this->id; }
	D3DCOLOR GetBackgroundColor() { return backgroundColor; }

	std::vector<LPGameObject> GetObjects();
		void AddObject(LPGameObject gameObject);
	void RemoveObject(LPGameObject gameObject);

	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;

	virtual ~CScene();
};
#endif
