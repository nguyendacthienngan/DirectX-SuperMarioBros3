#pragma once
#include "GameObject.h"
class CCoin : public CGameObject
{
public:
	CCoin();
	void LoadAnimation();
	void Init();
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
};
