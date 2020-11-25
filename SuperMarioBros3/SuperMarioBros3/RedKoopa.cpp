#include "RedKoopa.h"
#include "AnimationManager.h"
#include "KoopaConst.h"
#include "Ultis.h"
CRedKoopa::CRedKoopa()
{
	LoadAnimation();
	Init();
	koopaType = KoopaType::Red;
}

void CRedKoopa::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	CKoopa::Update(dt, cam, uiCam);
}

void CRedKoopa::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	CKoopa::OnCollisionEnter(selfCollisionBox, collisionEvents);
}

void CRedKoopa::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(KOOPA_STATE_MOVE, animationManager->Clone("ani-red-koopa-troopa-move"));
	AddAnimation(KOOPA_STATE_WITH_DRAW, animationManager->Clone("ani-red-koopa-troopa-with-draw"));
}

