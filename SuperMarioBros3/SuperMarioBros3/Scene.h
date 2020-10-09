#pragma once

#ifndef SCENE_H
#define SCENE_H

#include "GameObject.h"

class CScene;
typedef CScene* LPScene;

class CScene
{
protected:
	std::vector<LPGameObject> gameObjects;
	std::string id;
	LPCWSTR filePath;
public:
	CScene();

	virtual void Load() = 0;
	virtual void Unload();

	virtual void Update(DWORD dt) = 0;
	virtual void Render();

	std::string GetSceneId() { return this->id; }

	void AddObject(LPGameObject gameObject);
	void RemoveObject(LPGameObject gameObject);

	/*virtual void KeyState(BYTE* state) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;*/

	virtual ~CScene();
};
#endif
