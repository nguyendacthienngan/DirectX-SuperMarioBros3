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
		case 0:
		{
			if (curtain == NULL)
			{
				curtain = new CCurtain();
				auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
				activeScene->AddObject(curtain);
			}
			cutSceneStages++;
			
			break;
		}
		case 1:
		{
			if (marioScript == NULL)
			{
				marioScript = new CMarioScript();
				auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
				activeScene->AddObject(marioScript);
			}
			if (curtain->GetPosition().y + 250 < cam->GetPositionCam().y)
			{
				marioScript->SetStartAction();
				curtain->Enable(false);
			}
			if (marioScript->GetLuigiPosition().x > 600)
			{
				cutSceneStages++;
			}
			break;
			
		}
		case 2:
		{
			CIntroTitle* title = new CIntroTitle();
			auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
			activeScene->AddObject(title);
			cutSceneStages++;
			break;
		}
	}
	
}

void CIntroScript::Render(CCamera* cam, int alpha)
{
	switch (cutSceneStages)
	{
		case 0:
		{
		}
	}
}
