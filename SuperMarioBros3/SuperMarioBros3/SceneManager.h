#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "UICamera.h"

class CSceneManager;
typedef CSceneManager* LPSceneManager;
class CSceneManager
{
	static LPSceneManager instance;
	std::unordered_map<std::string, LPScene> loadedScenes;
	std::string activeSceneId;
	CUICamera* uiCamera;
	std::vector<LPScene> requestedLoadScene;
	std::vector<LPScene> requestedUnloadScene;
	int currentNodeID;
public:
	CSceneManager();
	static LPSceneManager GetInstance();

	void Init();

	void Load(LPScene scene);
	void Unload(std::string sceneId);

	LPScene GetScene(std::string id);
	CUICamera* GetUICamera();

	std::string GetActiveSceneId();
	LPScene GetActiveScene();

	int GetNodeID();
	void SetNodeID(int id);

	void LoadRequestScene();
	void UnloadRequestScene();
	void SwitchScene(LPScene scene);
	~CSceneManager();
};

#endif