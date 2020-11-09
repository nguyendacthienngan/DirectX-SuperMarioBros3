#include "Enemy.h"

CEnemy::CEnemy()
{
	this->SetTag(GameObjectTags::Enemy);
	this->SetScale(D3DXVECTOR2(1.0f, 1.0f));
	countDeadCallback = 0;
}

void CEnemy::SetEnemyType(std::string eT)
{
	enemyType = eT;
}

std::string CEnemy::GetEnemyType()
{
	return enemyType;
}

void CEnemy::SetEnemyTag(EnemyTag eT)
{
	enemyTag = eT;
}

EnemyTag CEnemy::GetEnemyTag()
{
	return enemyTag;
}

void CEnemy::SetStartPosition(D3DXVECTOR2 sT)
{
	this->startPosition = sT;
}

D3DXVECTOR2 CEnemy::GetStartPosition()
{
	return startPosition;
}

void CEnemy::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Enemy )
		{
			auto otherObject = collisionBox->GetGameObjectAttach();
			auto otherEnemyObject = static_cast<CEnemy*>(otherObject);

			if (otherEnemyObject->GetEnemyTag() == this->enemyTag && collisionEvent->nx != 0)
			{
				auto normal = physiscBody->GetNormal();
				normal.x = -1;
				physiscBody->SetNormal(normal);
			}
		}
	}
}
