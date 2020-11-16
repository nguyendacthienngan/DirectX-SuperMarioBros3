#include "QuestionBlock.h"
#include "AnimationManager.h"
#include "QuestionBlockConst.h"
#include "CoinEffect.h"
#include "SceneManager.h"
#include "Ultis.h"
#include "Game.h"
CQuestionBlock::CQuestionBlock()
{
	CBlock::CBlock();
	SetTag(GameObjectTags::QuestionBlock);
	LoadAnimation();
	SetState(QB_STATE_SEALED);
	isEnabled = true;
	bounceState = 0;
	countBounceTime = 0;
	startBounceTime = 0;
	bounceDelta = 0.0f;
}

void CQuestionBlock::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation(QB_STATE_SEALED, animations->Get("ani-question-block"));
	AddAnimation(QB_STATE_EMPTY, animations->Get("ani-empty-block"));
}

void CQuestionBlock::Init()
{

}


void CQuestionBlock::SetItemInfo(ItemInfo info)
{
	this->itemInfo = info;
}

void CQuestionBlock::Bounce()
{
	if (bounceState == 0)
	{
		bounceState = 1;
		countBounceTime++;
		if (countBounceTime == 1) // Tạm thời
		{
			CCoinEffect* coinObtainedFX = new CCoinEffect();
			coinObtainedFX->SetStartPosition(transform.position);
			auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
			activeScene->AddObject(coinObtainedFX);
			startBounceTime = GetTickCount64();
		}
	}
}

void CQuestionBlock::Update(DWORD dt, CCamera* cam)
{
	if (bounceState == 2)
	{
		DebugOut(L"BOUNCESTATE 2 \n");
		bounceDelta = BOUNCE_VEL;

		if (GetTickCount64() - startBounceTime > BOUNCE_TIME && startBounceTime != 0)
		{
			bounceState = -1;
			bounceDelta = 0.0f;
			startBounceTime = 0;
		}
	}
	if (bounceState == 1)
	{
		DebugOut(L"BOUNCESTATE 1 \n");
		bounceDelta = -BOUNCE_VEL;

		if (GetTickCount64() - startBounceTime > BOUNCE_TIME && startBounceTime != 0)
		{
			bounceState = 2;
			startBounceTime = GetTickCount64();
		}
	}
	
}

void CQuestionBlock::Render(CCamera* cam, int alpha)
{
	relativePositionOnScreen.y += bounceDelta;
	if (bounceState != 0)
		SetState(QB_STATE_EMPTY); 
	else
		SetState(QB_STATE_SEALED);
	CGameObject::Render(cam, alpha);
}
