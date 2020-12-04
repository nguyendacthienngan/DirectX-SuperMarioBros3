#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Coin.h"
class CPSwitch : public CGameObject
{
private:
	std::vector<CBrick*> listBricks;
	std::vector<CCoin*> listCoins;
	DWORD timer;
	int changeState; // 0: inactive, 1: change bricks to coins, 2: change coins to bricks
public:
	CPSwitch();
	void LoadAnimation() override;
	void Render(CCamera* cam, int alpha);
	virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Active();
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag);
	void SetListBricks(std::vector<CBrick*> listBricks);
	void SetListCoins(std::vector<CCoin*> listCoins);
};

