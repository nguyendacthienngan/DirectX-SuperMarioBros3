#include "FireFlower.h"
#include "AnimationManager.h"

CFireFlower::CFireFlower()
{
	LoadAnimation();
	SetState("FIREFLOWER");
	SetScale(D3DXVECTOR2(0.4f, 1.0f));
	SetTag(GameObjectTags::Gift);
	itemTag = ItemTag::PowerUp;
	powerupTag = PowerupTag::FireFlower;
	isEnabled = true;

	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(D3DXVECTOR2(48,48));
	box->SetGameObjectAttach(this);
	box->SetEnable(true);
	this->collisionBoxs->push_back(box);
}

void CFireFlower::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("FIREFLOWER", animations->Get("ani-fire-flower"));
}

bool CFireFlower::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (MarioTag(tag))
		return true;
	return false;
}
