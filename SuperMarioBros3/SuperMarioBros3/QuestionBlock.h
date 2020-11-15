#pragma once
#include "Block.h"
class CQuestionBlock :  public CBlock
{
private:
	int amountOfCoinCanAchive;
public:
	CQuestionBlock();
	void LoadAnimation() override;
	void Init() override;
	void SetAmountOfCoinCanAchive(int amount);
};

