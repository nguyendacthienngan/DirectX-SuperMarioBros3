#pragma once
#include "GameObject.h"
class CBrick : public CGameObject
{
	int id, type;
public:
	CBrick();
	void LoadAnimation();
	void Init();
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
	void SetID(int id);
	int GetID();
	void SetType(int type);
	int GetType();
};

