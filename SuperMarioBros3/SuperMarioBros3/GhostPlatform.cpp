#include "GhostPlatform.h"

CGhostPlatform::CGhostPlatform()
{
	Init();
}

void CGhostPlatform::Init()
{
	CCollisionBox* box = new CCollisionBox();
	box->SetGameObjectAttach(this);
	this->collisionBoxs->push_back(box);
	this->physiscBody->SetDynamic(false);
	this->SetTag(GameObjectTags::GhostPlatform);
	this->SetEffector(Effector::Top);
}
