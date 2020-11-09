#include "RedKoopa.h"

void CRedKoopa::Update(DWORD dt, CCamera* cam)
{
	CKoopa::Update(dt, cam);

}

void CRedKoopa::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	CKoopa::OnCollisionEnter(selfCollisionBox, collisionEvents);
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionEvent->ny >= 0 || collisionEvent->nx != 0)
			continue;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Solid || collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::GhostPlatform)
		{
			auto otherBBox = collisionBox->GetBoundingBox();
			auto normal = physiscBody->GetNormal();
			auto velocity = physiscBody->GetVelocity();
			if (transform.position.x > otherBBox.right)
			{
				normal.x = -1;
				velocity.x = -velocity.x;
				transform.position.x = otherBBox.right - 1;
			}
			if (transform.position.x < otherBBox.left)
			{
				transform.position.x = otherBBox.left + 1;
				normal.x = 1;
				velocity.x = -velocity.x;
			}
			physiscBody->SetNormal(normal);
			physiscBody->SetVelocity(velocity);
		}
	}
}
