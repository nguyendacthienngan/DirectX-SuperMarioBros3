#include "RedParaKoopa.h"
#include "AnimationManager.h"
#include "ParaKoopaConst.h"
const D3DXVECTOR2 PARAKOOPA_VELOCITY = D3DXVECTOR2(0, 0.1f);
CRedParaKoopa::CRedParaKoopa()
{
	LoadAnimation();
	SetState(PARAKOOPA_STATE_FLY);
	physiscBody->SetVelocity(PARAKOOPA_VELOCITY);
}

void CRedParaKoopa::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(PARAKOOPA_STATE_FLY, animationManager->Clone("ani-red-koopa-paratroopa-fly"));
}

void CRedParaKoopa::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto vel = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	auto velDefault = PARAKOOPA_VELOCITY.y;
	if (GetPosition().y < boundaryTop || GetPosition().y > boundaryBottom)
		normal.y = -normal.y;

	vel.y = velDefault * normal.y;
	physiscBody->SetVelocity(vel);

	physiscBody->SetNormal(normal);
}

void CRedParaKoopa::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}

void CRedParaKoopa::SetBoundary(float bTop, float bBottom)
{
	boundaryTop = bTop;
	boundaryBottom = bBottom;
}
