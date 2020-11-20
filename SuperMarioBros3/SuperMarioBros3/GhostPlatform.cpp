#include "GhostPlatform.h"
#include "Enemy.h"

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
	this->isEnabled = true;
}

bool CGhostPlatform::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (tag == GameObjectTags::Enemy)
	{
		auto enemy = static_cast<CEnemy*>(gO);
		if (enemy->GetEnemyTag() == EnemyTag::Piranha || enemy->GetEnemyTag() == EnemyTag::Venus)
			return false;
	}
	return true;
}

CGhostPlatform::~CGhostPlatform()
{
	CGameObject::~CGameObject();

}
