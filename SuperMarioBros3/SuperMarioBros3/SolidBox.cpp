#include "SolidBox.h"

CSolidBox::CSolidBox()
{
	Init();
}

void CSolidBox::Init()
{
	CCollisionBox* box = new CCollisionBox();
	box->SetGameObjectAttach(this);
	this->collisionBoxs->push_back(box);
	this->physiscBody->SetDynamic(false);
}
