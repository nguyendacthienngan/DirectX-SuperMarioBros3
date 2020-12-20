#include "ScoreEffect.h"
#include "ScoreFXConst.h"
#include "Game.h"
#include "UICamera.h"
#include "AnimationManager.h"
CScoreEffect::CScoreEffect()
{
	LoadAnimation();
	currentScore = 100;
	SetState("100");
	physiscBody->SetGravity(SCORE_FX_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, -SCORE_FX_VELOCITY));
	physiscBody->SetDynamic(false);
	isEnabled = true;
	scoreState = 0;
}

void CScoreEffect::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("100", animationManager->Get("ani-points-100"));
	AddAnimation("200", animationManager->Get("ani-points-200"));
	AddAnimation("400", animationManager->Get("ani-points-400"));
	AddAnimation("800", animationManager->Get("ani-points-800"));
	AddAnimation("1000", animationManager->Get("ani-points-1000"));
	AddAnimation("2000", animationManager->Get("ani-points-2000"));
	AddAnimation("4000", animationManager->Get("ani-points-4000"));
	AddAnimation("8000", animationManager->Get("ani-points-8000"));
	AddAnimation("1UP", animationManager->Get("ani-points-1UP"));
}


void CScoreEffect::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	scoreState++;
	transform.position.y -= SCORE_FX_VELOCITY * dt;
	if (uiCam != NULL && scoreState == 1)
	{
		auto uiCamera = static_cast<CUICamera*>(uiCam);
		uiCamera->GetHUD()->AddingScore(currentScore);
		uiCamera->GetHUD()->AddingCoin(currentScore / 100);
	}
	// startPos - currentPos <= MAX_HEIGHT
	if (startPosition.y - transform.position.y > SCORE_FX_APPEARING_HEIGHT)
		isEnabled = false;
}

void CScoreEffect::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}
