#include "GreenKoopaShell.h"
#include "AnimationManager.h"
#include "KoopaConst.h"
#include "Ultis.h"

CGreenKoopaShell::CGreenKoopaShell()
{
	LoadAnimation();
	SetState(KOOPA_SHELL_STATE_IDLE);
}

void CGreenKoopaShell::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(KOOPA_SHELL_STATE_IDLE, animationManager->Get("ani-green-koopa-troopa-shell-idle"));
	AddAnimation(KOOPA_SHELL_STATE_RUN, animationManager->Get("ani-green-koopa-troopa-shell-run"));
	AddAnimation(KOOPA_SHELL_STATE_WITHDRAW, animationManager->Get("ani-green-koopa-troopa-with-draw"));
}
