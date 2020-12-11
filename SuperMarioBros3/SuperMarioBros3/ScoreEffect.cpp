#include "ScoreEffect.h"
#include "ScoreFXConst.h"
#include "Game.h"
#include "UICamera.h"
CScoreEffect::CScoreEffect()
{
	scoreText = new CFont();
	scoreText->SetCurrentText("100");
	currentScore = 100;
	scoreText->SetSpace(D3DXVECTOR2(8*3, 0));
	physiscBody->SetGravity(SCORE_FX_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, -SCORE_FX_VELOCITY));
	physiscBody->SetDynamic(false);
	isEnabled = true;
	scoreState = 0;
}

void CScoreEffect::SetStartPosition(D3DXVECTOR2 pos)
{
	CEffectObject::SetStartPosition(pos);
}

void CScoreEffect::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	if (scoreText == NULL)
		return;
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
	auto camPos = cam->Transform(GetPosition());
	scoreText->SetPosition(camPos);
	scoreText->Render();
}
