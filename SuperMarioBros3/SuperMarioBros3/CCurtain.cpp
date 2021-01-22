#include "CCurtain.h"
#include "SpriteManager.h"
#include "Game.h"

#define CURTAIN_SPEED 0.2

CCurtain::CCurtain()
{
	auto spriteManager = CSpriteManager::GetInstance();
	curtainSprite = spriteManager->Get("spr-full-curtain-0");
	pos = D3DXVECTOR2(384, 300);
}

void CCurtain::Update()
{
	auto dt = CGame::GetInstance()->GetDeltaTime();
	pos.y -= CURTAIN_SPEED * dt;

}

void CCurtain::Render()
{
	curtainSprite->Draw(pos, D3DXVECTOR2(5.0f, 5.0f), 0.0f);
}
