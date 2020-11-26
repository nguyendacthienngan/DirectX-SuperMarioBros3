#include "MarioMap.h"
#include "AnimationManager.h"
#include "MarioMapConst.h"
#include "KeyboardManager.h"
CMarioMap::CMarioMap()
{
	LoadAnimation();
	SetState(SMALL_MARIO_MAP_STATE);
	isEnabled = true;
	physiscBody->SetDynamic(false);
}

void CMarioMap::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(SMALL_MARIO_MAP_STATE, animationManager->Get("ani-small-mario-map"));
	AddAnimation(BIG_MARIO_MAP_STATE, animationManager->Get("ani-big-mario-map"));
	AddAnimation(RACCOON_MARIO_MAP_STATE, animationManager->Get("ani-raccoon-mario-map"));
	AddAnimation(FIRE_MARIO_MAP_STATE, animationManager->Get("ani-fire-mario-map"));
}

void CMarioMap::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	/*auto keyboard = CKeyboardManager::GetInstance();
	if (keyboard->GetKeyStateDown(DIK_RIGHT))
	{
		transform.position.x += 0.02 * dt;
	}*/
}

void CMarioMap::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}
