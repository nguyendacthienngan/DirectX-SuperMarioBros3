#include "CCurtain.h"
#include "SpriteManager.h"
#include "Game.h"

#define CURTAIN_SPEED 0.2

CCurtain::CCurtain()
{
	auto spriteManager = CSpriteManager::GetInstance();
	curtainSprite = spriteManager->Get("spr-full-curtain-0");
	isEnabled = true;
	transform.position = D3DXVECTOR2(384, 300);
}

void CCurtain::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	transform.position.y -= CURTAIN_SPEED * dt;
}

void CCurtain::Render(CCamera* cam, int alpha)
{

	auto curtainPos = transform.position - cam->GetPositionCam();
	curtainSprite->Draw(curtainPos , D3DXVECTOR2(1.0f, 1.0f), 0);

}