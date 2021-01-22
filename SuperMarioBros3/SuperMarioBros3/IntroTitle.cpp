#include "IntroTitle.h"
#include "SpriteManager.h"
#include <cstdlib>

#include <cmath>
#include "ThreeAnimation.h"
#include "SceneManager.h"

using namespace std;
const D3DXVECTOR2 DESTINATION_POS = D3DXVECTOR2(350, 200);
const int MOVEMENT_DURATION = 800;
CIntroTitle::CIntroTitle()
{
	auto spriteManager = CSpriteManager::GetInstance();
	titleSprites[0] = spriteManager->Get("spr-big-cloud-0");
	titleSprites[1] = spriteManager->Get("spr-small-cloud-0");
	titleSprites[2] = spriteManager->Get("spr-title-0");
	titleSprites[3] = spriteManager->Get("spr-three-1");
	isEnabled = true;
	transform.position = D3DXVECTOR2(350, -300); 
	titleAppearingState = 0;
	startPos = transform.position;
}

void CIntroTitle::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	switch (titleAppearingState)
	{
		case 0:
		{
			auto pos = transform.position;

			if (pos.y >= DESTINATION_POS.y)
			{
				SetPosition(DESTINATION_POS);
				titleAppearingState++;
			}
			else
			{
				auto distance = DESTINATION_POS - startPos;
				auto vel = sqrt(distance.x * distance.x + distance.y * distance.y) / MOVEMENT_DURATION;
				pos += D3DXVECTOR2(0, vel * dt);
				SetPosition(pos);
			}
			break;
		}
		case 1:
		{
			CThreeAnimation* three = new CThreeAnimation();
			three->SetPosition(transform.position  + D3DXVECTOR2(14, 140));
			CSceneManager::GetInstance()->GetActiveScene()->AddObject(three);
			titleAppearingState++;
			break;
		}
	}
}

void CIntroTitle::Render(CCamera* cam, int alpha)
{
	auto titlePos = transform.position - cam->GetPositionCam();
	titleSprites[2]->Draw(titlePos, D3DXVECTOR2(1,1), 0);

	auto cloudPos0 = titlePos + D3DXVECTOR2(-265, -50);
	titleSprites[0]->Draw(cloudPos0, D3DXVECTOR2(1, 1), 0);

	auto cloudPos1 = titlePos + D3DXVECTOR2(260, -104);
	titleSprites[0]->Draw(cloudPos1, D3DXVECTOR2(1, 1), 0);

	auto cloudPos2 = titlePos + D3DXVECTOR2(-350, 75);
	titleSprites[1]->Draw(cloudPos2, D3DXVECTOR2(1, 1), 0);

	auto versionPos = titlePos + D3DXVECTOR2(14, 156);
	titleSprites[3]->Draw(versionPos, D3DXVECTOR2(1, 1), 0);
}
