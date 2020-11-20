#pragma once
#include "GameObject.h"
class CEffectObject: public CGameObject
{
protected:
	D3DXVECTOR2 startPosition;

public:
	CEffectObject();
	void SetStartPosition(D3DXVECTOR2 pos);
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
};

