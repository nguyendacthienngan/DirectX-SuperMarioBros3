#pragma once
#include "Block.h"
class CEmptyBlock :  public CBlock
{
public:
	CEmptyBlock();
	void LoadAnimation() override;
};

