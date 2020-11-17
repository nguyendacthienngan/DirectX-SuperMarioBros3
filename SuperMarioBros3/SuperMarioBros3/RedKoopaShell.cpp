#include "RedKoopaShell.h"
#include "KoopaConst.h"
#include "AnimationManager.h"
CRedKoopaShell::CRedKoopaShell()
{
	LoadAnimation();
}
void CRedKoopaShell::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(KOOPA_SHELL_STATE_IDLE, animationManager->Get("ani-red-koopa-troopa-shell-idle"));
	AddAnimation(KOOPA_SHELL_STATE_RUN, animationManager->Get("ani-red-koopa-troopa-shell-run"));
	AddAnimation(KOOPA_SHELL_STATE_WITHDRAW, animationManager->Get("ani-red-koopa-troopa-with-draw"));
}
