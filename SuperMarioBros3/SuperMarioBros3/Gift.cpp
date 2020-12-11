#include "Gift.h"
#include "Ultis.h"
#include "Mario.h"

CGift::CGift()
{
}

void CGift::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (MarioTag(collisionBox->GetGameObjectAttach()->GetTag()))
		{
			// Đụng trúng lá đỏ là đổi level và disable lá
			PowerUp(collisionBox);
		}
		if (powerupTag == PowerupTag::SuperMushroom && StaticTag(collisionBox->GetGameObjectAttach()->GetTag()))
		{
			if (collisionEvent->nx != 0)
			{
				auto normal = physiscBody->GetNormal();
				normal.x = -normal.x;
				physiscBody->SetNormal(normal);
			}
		}
	}
}

void CGift::OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	
}

void CGift::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (MarioTag(otherCollisionBox->GetGameObjectAttach()->GetTag()))
	{
		PowerUp(otherCollisionBox);
	}
	if (powerupTag == PowerupTag::SuperMushroom && StaticTag(otherCollisionBox->GetGameObjectAttach()->GetTag()))
	{
		auto normal = physiscBody->GetNormal();
		normal.x = -normal.x;
		physiscBody->SetNormal(normal);
	}
}

bool CGift::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	return MarioTag(tag);
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
	mario->SetPowerUpItem(powerupTag);
	mario->SetPowerUp(true);
}
