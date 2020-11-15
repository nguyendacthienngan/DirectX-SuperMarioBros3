#pragma once
#include "Block.h"
#include "ItemInfo.h"
class CQuestionBlock :  public CBlock
{
private:
	ItemInfo itemInfo;
	int countBounceTime;
	bool isBounced;
public:
	CQuestionBlock();
	void LoadAnimation() override;
	void Init() override;
	void SetItemInfo(ItemInfo info);
	void Bounce();
	virtual void Render(CCamera* cam, int alpha = 255);

};

