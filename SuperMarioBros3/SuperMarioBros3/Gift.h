#pragma once
#include "EffectObject.h"
#include "ItemInfo.h"
class CGift : public CEffectObject
{
protected:
	ItemTag itemTag;
public:
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions); // Xử lý riêng sau khi phát hiện va chạm
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	void PowerUp(CCollisionBox* otherCollisionBox);
};

