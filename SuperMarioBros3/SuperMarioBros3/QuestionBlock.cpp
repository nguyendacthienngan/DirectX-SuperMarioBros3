#include "QuestionBlock.h"
#include "AnimationManager.h"
#include "QuestionBlockConst.h"
#include "CoinEffect.h"
#include "SceneManager.h"
#include "Ultis.h"
CQuestionBlock::CQuestionBlock()
{
	CBlock::CBlock();
	SetTag(GameObjectTags::QuestionBlock);
	LoadAnimation();
	SetState(QB_STATE_SEALED);
	isEnabled = true;
	isBounced = false;
	countBounceTime = 0;
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
	DebugOut(L"BOUNCE \n");
	isBounced = true;
	countBounceTime++;
	if (countBounceTime == 1) // Tạm thời
	{
		CCoinEffect* coinObtainedFX = new CCoinEffect();
		coinObtainedFX->SetStartPosition(transform.position);
		auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
		activeScene->AddObject(coinObtainedFX);
	}
	
}

void CQuestionBlock::Render(CCamera* cam, int alpha)
{
	if (isBounced == true)
		SetState(QB_STATE_EMPTY); 
	else
		SetState(QB_STATE_SEALED);
	CGameObject::Render(cam, alpha);
}
