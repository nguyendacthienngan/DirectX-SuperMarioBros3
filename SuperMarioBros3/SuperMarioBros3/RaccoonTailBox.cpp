#include "RaccoonTailBox.h"
#include "RaccoonTailConst.h"
#include "MarioConst.h"

CRaccoonTailBox::CRaccoonTailBox()
{
	this->isEnabled = false;
	Init();
}

void CRaccoonTailBox::Init()
{
	this->tag = GameObjectTags::RaccoonTail;
	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(RACCOON_TAIL_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Raccoon-Tail"); 
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);

	this->SetScale(D3DXVECTOR2(1.0f, 1.0f));
}

void CRaccoonTailBox::Update(DWORD dt, CCamera* cam)
{
	// Di chuyển theo mario
	// Nhưng đặt phía trước mario như thế nào
	/*auto velocity = physiscBody->GetVelocity();
	velocity = raccoonMario->GetPhysiscBody()->GetVelocity();
	physiscBody->SetVelocity(velocity);*/
}

void CRaccoonTailBox::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	//for (auto collisionEvent : collisionEvents)
	//{
	//	auto collisionBox = collisionEvent->obj;
	//	if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Enemy)
	//	{
	//		if (collisionEvent->nx != 0)
	//		{
	//			// Quẫy đuôi trúng enemy
	//			// Enemy sẽ chết
	//			
	//		}
	//	}
	//}
}
