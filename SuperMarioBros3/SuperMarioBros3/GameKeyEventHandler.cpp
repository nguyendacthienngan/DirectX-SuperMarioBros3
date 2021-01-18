#include <dinput.h>

#include "GameKeyEventHandler.h"
#include "Ultis.h"
#include "SceneManager.h"
#include "GameObject.h"

using namespace std;
void CGameKeyEventHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	for (auto gameObject : currentTargets)
		if (gameObject != NULL && gameObject->IsEnabled() == true)
		gameObject->OnKeyDown(KeyCode);
}

void CGameKeyEventHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	for (auto gameObject : currentTargets)
		if (gameObject != NULL && gameObject->IsEnabled() == true)
		gameObject->OnKeyUp(KeyCode);
}

void CGameKeyEventHandler::KeyState()
{
	for (auto gameObject : currentTargets)
		if (gameObject != NULL && gameObject->IsEnabled() == true)
		gameObject->KeyState();
}