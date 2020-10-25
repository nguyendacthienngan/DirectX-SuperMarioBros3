#include "SolidBox.h"
#include "Ultis.h"
#include "TextureManager.h"
#include "Const.h"
#include "Game.h"

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
	this->SetTag(GameObjectTags::Solid);
}
