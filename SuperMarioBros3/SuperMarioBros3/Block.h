#pragma once
#include "GameObject.h"
class CBlock : public CGameObject
{
public:
	CBlock();
	virtual void Init();
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
};

