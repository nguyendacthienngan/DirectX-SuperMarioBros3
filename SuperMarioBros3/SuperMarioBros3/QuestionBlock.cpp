#include "QuestionBlock.h"
#include "AnimationManager.h"
#include "QuestionBlockConst.h"
CQuestionBlock::CQuestionBlock()
{
	CBlock::CBlock();
	LoadAnimation();
	SetState(QB_STATE_SEALED);
	amountOfCoinCanAchive = 0;
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

void CQuestionBlock::SetAmountOfCoinCanAchive(int amount)
{
	this->amountOfCoinCanAchive = amount;
}
