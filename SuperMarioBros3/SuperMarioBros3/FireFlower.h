#pragma once
#include "Gift.h"
class CFireFlower : public CGift
{
public:
	CFireFlower();
	void LoadAnimation();
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
};

