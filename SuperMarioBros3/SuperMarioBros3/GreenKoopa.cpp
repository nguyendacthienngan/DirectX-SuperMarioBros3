#include "GreenKoopa.h"
#include "AnimationManager.h"
#include "KoopaConst.h"

CGreenKoopa::CGreenKoopa()
{
	CGreenKoopa::LoadAnimation();
	SetState(KOOPA_STATE_MOVE);
	Init();
	isEnabled = false;
	koopaType = KoopaType::Green;
}

void CGreenKoopa::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(KOOPA_STATE_MOVE, animationManager->Clone("ani-green-koopa-troopa-move"));
	AddAnimation(KOOPA_STATE_WITH_DRAW, animationManager->Clone("ani-green-koopa-troopa-with-draw"));
}
