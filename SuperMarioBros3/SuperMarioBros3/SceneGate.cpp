#include "SceneGate.h"
#include "AnimationManager.h"
#include "Ultis.h"

CSceneGate::CSceneGate(D3DXVECTOR2 size)
{
	Init(size);
	LoadAnimation();
	SetState("SCENE-1");
}

void CSceneGate::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("SCENE-1",animationManager->Get("ani-gate-1"));
}

void CSceneGate::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}
