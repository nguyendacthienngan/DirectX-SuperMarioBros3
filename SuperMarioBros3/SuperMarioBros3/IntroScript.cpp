#include "IntroScript.h"
#include "IntroTitle.h"
#include "SceneManager.h"
#include "MarioScript.h"
CIntroScript::CIntroScript()
{
	isEnabled = true;
	transform.position = D3DXVECTOR2(200, 200); // Để được check trong camera
	cutSceneStages = 0;
}

void CIntroScript::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	switch (cutSceneStages)
	{
		case 1:
		{
			CIntroTitle* title = new CIntroTitle();
			auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
			activeScene->AddObject(title);
			cutSceneStages++;
			break;
		}
		case 0:
		{
			CMarioScript* script = new CMarioScript();
			auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
			activeScene->AddObject(script);
			cutSceneStages++;
		}
	}
	
}

void CIntroScript::Render(CCamera* cam, int alpha)
{

}
