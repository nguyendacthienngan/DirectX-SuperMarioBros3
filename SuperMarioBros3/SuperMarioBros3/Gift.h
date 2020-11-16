#pragma once
#include "EffectObject.h"
class CGift : public CEffectObject
{
public:
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions); // Xử lý riêng sau khi phát hiện va chạm
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;

};

