﻿#include <dinput.h>

#include "GameKeyEventHandler.h"
#include "Ultis.h"
#include "SceneManager.h"
#include "GameObject.h"

using namespace std;
void CGameKeyEventHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	LPSceneManager sceneManger = CSceneManager::GetInstance();
	LPScene activeScene = sceneManger->GetActiveScene();
	vector<LPGameObject> gameObjects = activeScene->GetObjects(); // nếu vậy mình chỉ truyền cho thằng nào dynamic cast ra là Mario thôi đc k vì mấy thằng kia mình đâu cần xử lý? Nhưng như vậy sẽ không đủ tổng quát? Nên hỏi thầy !
	for (auto gameObject : gameObjects)
		if (gameObject->GetTag() == GameObjectTags::Player || gameObject->GetTag() == GameObjectTags::SmallMario 
			|| gameObject->GetTag() == GameObjectTags::SuperMario || gameObject->GetTag() == GameObjectTags::FireMario
			|| gameObject->GetTag() == GameObjectTags::RaccoonMario)
				gameObject->OnKeyDown(KeyCode);
}

void CGameKeyEventHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	LPSceneManager sceneManger = CSceneManager::GetInstance();
	LPScene activeScene = sceneManger->GetActiveScene();
	vector<LPGameObject> gameObjects = activeScene->GetObjects(); // nếu vậy mình chỉ truyền cho thằng nào dynamic cast ra là Mario thôi đc k vì mấy thằng kia mình đâu cần xử lý? Nhưng như vậy sẽ không đủ tổng quát? Nên hỏi thầy !
	for (auto gameObject : gameObjects)
		if (gameObject->GetTag() == GameObjectTags::Player || gameObject->GetTag() == GameObjectTags::SmallMario
			|| gameObject->GetTag() == GameObjectTags::SuperMario || gameObject->GetTag() == GameObjectTags::FireMario
			|| gameObject->GetTag() == GameObjectTags::RaccoonMario)
				gameObject->OnKeyUp(KeyCode);
}

void CGameKeyEventHandler::KeyState()
{
	//DebugOut(L"[INFO] KeyState: %d\n");
	LPSceneManager sceneManger = CSceneManager::GetInstance();
	LPScene activeScene = sceneManger->GetActiveScene();
	vector<LPGameObject> gameObjects = activeScene->GetObjects(); // nếu vậy mình chỉ truyền cho thằng nào dynamic cast ra là Mario thôi đc k vì mấy thằng kia mình đâu cần xử lý? Nhưng như vậy sẽ không đủ tổng quát? Nên hỏi thầy !
	for (auto gameObject : gameObjects)
		if (gameObject->GetTag() == GameObjectTags::Player || gameObject->GetTag() == GameObjectTags::SmallMario
			|| gameObject->GetTag() == GameObjectTags::SuperMario || gameObject->GetTag() == GameObjectTags::FireMario
			|| gameObject->GetTag() == GameObjectTags::RaccoonMario)
				gameObject->KeyState();
}
