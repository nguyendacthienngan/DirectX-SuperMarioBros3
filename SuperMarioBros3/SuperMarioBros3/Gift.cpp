#include "Gift.h"
#include "Ultis.h"
#include "Mario.h"

void CGift::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Player)
		{
			// Đụng trúng lá đỏ là đổi level và disable lá
			PowerUp(collisionBox);
		}
	}
	
}

void CGift::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Player)
	{
		PowerUp(otherCollisionBox);
	}
}

void CGift::PowerUp(CCollisionBox* otherCollisionBox)
{
	isEnabled = false;
	GetPhysiscBody()->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	GetPhysiscBody()->SetGravity(0.0f);
	this->collisionBoxs->at(0)->SetEnable(false);

	auto gO = otherCollisionBox->GetGameObjectAttach();
	auto mario = static_cast<CMario*>(gO);
	mario->OnDamaged();
	mario->SetPowerUpItem(itemTag);
	mario->SetPowerUp(true);
}
