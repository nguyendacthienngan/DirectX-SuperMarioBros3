#pragma once
#include "Block.h"
#include "ItemInfo.h"
class CQuestionBlock :  public CBlock
{
private:
	ItemInfo itemInfo;
	int bounceState;
	float bounceDelta;
	DWORD startBounceTime;
public:
	CQuestionBlock();
	void LoadAnimation() override;
	void Init() override;
	void SetItemInfo(ItemInfo info);
	void Bounce(CGameObject* gO);
	virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	virtual void Render(CCamera* cam, int alpha = 255);
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
};

