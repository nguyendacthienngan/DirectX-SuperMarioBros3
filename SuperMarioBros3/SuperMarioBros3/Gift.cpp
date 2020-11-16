#include "Gift.h"
#include "Ultis.h"

void CGift::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Player)
		{
			// Đụng trúng lá đỏ là đổi level và disable lá
			isEnabled = false;
			GetPhysiscBody()->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
			GetPhysiscBody()->SetGravity(0.0f);
			selfCollisionBox->SetEnable(false);
		}
	}
	
}

void CGift::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Player)
	{
		isEnabled = false;
		GetPhysiscBody()->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
		GetPhysiscBody()->SetGravity(0.0f);
		selfCollisionBox->SetEnable(false);
	}
}
