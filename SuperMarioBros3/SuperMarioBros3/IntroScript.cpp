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
			if (introTitle == NULL)
			{
				introTitle = new CIntroTitle();
				auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
				activeScene->AddObject(introTitle);
				cutSceneStages++;
			}
			break;
		}
		case 3:
		{
			if (introTitle->GetAppearState() == 2)
			{
				auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
				activeScene->SetBackgroundColor(D3DCOLOR_ARGB(255, 255, 219, 161));

				CMenu* menu = new CMenu();
				activeScene->AddObject(menu);
				activeScene->AddKeyboardTargetObject(menu);
				cutSceneStages++;
			}
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
