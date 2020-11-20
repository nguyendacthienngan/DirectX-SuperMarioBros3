#include "SolidBox.h"
#include "Ultis.h"
#include "TextureManager.h"
#include "Const.h"
#include "Game.h"
#include "Enemy.h"
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

bool CSolidBox::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (tag == GameObjectTags::Enemy)
	{
		auto enemy = static_cast<CEnemy*>(gO);
		if (enemy->GetEnemyTag() == EnemyTag::Piranha || enemy->GetEnemyTag() == EnemyTag::Venus)
			return false;
	}
	return true; 
}

CSolidBox::~CSolidBox()
{
	CGameObject::~CGameObject();

}
