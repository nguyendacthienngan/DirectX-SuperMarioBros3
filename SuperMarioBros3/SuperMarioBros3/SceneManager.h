#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"

class CSceneManager;
typedef CSceneManager* LPSceneManager;
class CSceneManager
{
	static LPSceneManager instance;
	std::unordered_map<std::string, LPScene> loadedScenes;
	std::string activeSceneId;

public:
	CSceneManager();
	static LPSceneManager GetInstance();

	void Init();

	void Load(LPScene scene);
	void Unload(std::string sceneId);

	LPScene GetScene(std::string id);

	std::string GetActiveSceneId();
	LPScene GetActiveScene();

	~CSceneManager();
};

#endif