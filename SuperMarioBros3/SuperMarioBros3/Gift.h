#pragma once
#include "EffectObject.h"
#include "ItemInfo.h"
class CGift : public CEffectObject
{
protected:
	ItemTag itemTag;
	PowerupTag powerupTag;
public:
	CGift();
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions); // Xử lý riêng sau khi phát hiện va chạm
	void OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
	void PowerUp(CCollisionBox* otherCollisionBox);

	PowerupTag GetPowerupTag() { return powerupTag;  }
	void SetPowerupTag(PowerupTag pT) { this->powerupTag = pT; }
};

