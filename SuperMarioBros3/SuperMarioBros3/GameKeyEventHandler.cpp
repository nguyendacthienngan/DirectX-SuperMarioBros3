#include <dinput.h>

#include "GameKeyEventHandler.h"
#include "Ultis.h"
#include "SceneManager.h"
#include "GameObject.h"

using namespace std;
void GameKeyEventHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	LPSceneManager sceneManger = CSceneManager::GetInstance();
	LPScene activeScene = sceneManger->GetActiveScene();
	vector<LPGameObject> gameObjects = activeScene->GetObjects(); // nếu vậy mình chỉ truyền cho thằng nào dynamic cast ra là Mario thôi đc k vì mấy thằng kia mình đâu cần xử lý? Nhưng như vậy sẽ không đủ tổng quát? Nên hỏi thầy !
	for (auto gameObject : gameObjects)
		gameObject->OnKeyDown(KeyCode);
}

void GameKeyEventHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	LPSceneManager sceneManger = CSceneManager::GetInstance();
	LPScene activeScene = sceneManger->GetActiveScene();
	vector<LPGameObject> gameObjects = activeScene->GetObjects(); // nếu vậy mình chỉ truyền cho thằng nào dynamic cast ra là Mario thôi đc k vì mấy thằng kia mình đâu cần xử lý? Nhưng như vậy sẽ không đủ tổng quát? Nên hỏi thầy !
	for (auto gameObject : gameObjects)
		gameObject->OnKeyDown(KeyCode);
}
