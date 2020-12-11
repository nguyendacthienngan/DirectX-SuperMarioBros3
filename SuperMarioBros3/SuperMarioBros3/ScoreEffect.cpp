#include "ScoreEffect.h"
#include "ScoreFXConst.h"
#include "Game.h"
CScoreEffect::CScoreEffect()
{
	scoreText = new CFont();
	scoreText->SetCurrentText("100");
	scoreText->SetSpace(D3DXVECTOR2(8*3, 0));
	physiscBody->SetGravity(SCORE_FX_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, -SCORE_FX_VELOCITY));
	physiscBody->SetDynamic(false);
	isEnabled = true;
}

void CScoreEffect::SetStartPosition(D3DXVECTOR2 pos)
{
	CEffectObject::SetStartPosition(pos);
	//scoreText->SetPosition(pos);
}

void CScoreEffect::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	if (scoreText == NULL)
		return;
	
	transform.position.y -= SCORE_FX_VELOCITY * dt;
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
