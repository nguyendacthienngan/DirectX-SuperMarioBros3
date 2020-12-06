#pragma once
#include "GameObject.h"
class CCoin : public CGameObject
{
	int id, type;
public:
	CCoin();
	void LoadAnimation();
	void Init();
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
	void SetID(int id);
	int GetID();
	void SetType(int type);
	int GetType();
};
