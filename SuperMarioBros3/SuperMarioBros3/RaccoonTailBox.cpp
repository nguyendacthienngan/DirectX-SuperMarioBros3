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

