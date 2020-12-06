#pragma once
#include "GameObject.h"
class CBrick : public CGameObject
{
	int type;
public:
	CBrick();
	void LoadAnimation();
	void Init();
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
	void SetType(int type);
	int GetType();
};

