#pragma once
#include "GameObject.h"
class CBrick : public CGameObject
{
public:
	CBrick();
	void LoadAnimation();
	void Init();
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
};

