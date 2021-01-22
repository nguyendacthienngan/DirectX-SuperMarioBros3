#include "MarioScript.h"
#include "SceneManager.h"
#include "MarioControllerConst.h"

CMarioScript::CMarioScript()
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();

	marioController = new CMarioAuto();
	marioController->SetPosition(D3DXVECTOR2(743, 450));
	marioController->AddStateObjectsToScene(activeScene);
	marioController->SwitchToState(SUPER_MARIO_STATE);
	marioController->GetCurrentStateObject()->SetPosition(D3DXVECTOR2(743, 450));
	marioController->GetCurrentStateObject()->GetPhysiscBody()->SetNormal(D3DXVECTOR2(-1, 1));
	activeScene->AddObject(marioController);

	luigi = new CLuigiAuto();
	luigi->SetPosition(D3DXVECTOR2(24, 450));

	activeScene->AddObject(luigi);

	transform.position = D3DXVECTOR2(350, 300);

}

void CMarioScript::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	switch (stage)
	{
		case 0 :
		{
		}
	}
}

void CMarioScript::Render(CCamera* cam, int alpha)
{
}
