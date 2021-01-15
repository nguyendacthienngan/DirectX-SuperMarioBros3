#include "RedParaKoopa.h"
#include "AnimationManager.h"
#include "ParaKoopaConst.h"

CRedParaKoopa::CRedParaKoopa()
{
	LoadAnimation();
	SetState(PARAKOOPA_STATE_FLY);
}

void CRedParaKoopa::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(PARAKOOPA_STATE_FLY, animationManager->Clone("ani-red-koopa-paratroopa-fly"));
}

void CRedParaKoopa::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
}
