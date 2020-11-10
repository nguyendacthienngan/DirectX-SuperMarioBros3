#include "Enemy.h"
#include "Ultis.h"

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
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Enemy)
		{
			auto otherObject = collisionBox->GetGameObjectAttach();
			auto otherEnemyObject = static_cast<CEnemy*>(otherObject);
			if (collisionEvent->nx != 0)
			{
				// Nếu 2 quái cùng loại thì đổi chiều hướng đi
				// Khác loại thì xuyên qua nhưng khác loại mà 1 loại không phải mai rùa, 1 loại là mai rùa thì loại đó phải đi đổi chiều
				if (otherEnemyObject->GetEnemyTag() == this->enemyTag ||
					( this->enemyTag != EnemyTag::KoopaShell  && otherEnemyObject->GetEnemyTag() == EnemyTag::KoopaShell))
				{
					DebugOut(L"2 enemy gap nhau \n");

					auto normal = physiscBody->GetNormal();
					auto velocity = physiscBody->GetVelocity();
					normal.x = -normal.x;
					velocity.x *= normal.x;
					transform.position.x = velocity.x * dt;
					physiscBody->SetNormal(normal);
					physiscBody->SetVelocity(velocity);
				}
			}
			
		}
	}
}

void CEnemy::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	/*if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Enemy)
	{
		auto otherObject = otherCollisionBox->GetGameObjectAttach();
		auto otherEnemyObject = static_cast<CEnemy*>(otherObject);
		if (otherEnemyObject->GetEnemyTag() == this->enemyTag)
		{
			auto normal = physiscBody->GetNormal();
			auto velocity = physiscBody->GetVelocity();
			normal.x = -normal.x;
			velocity.x *= normal.x;
			transform.position.x = velocity.x * dt;
			physiscBody->SetNormal(normal);
			physiscBody->SetVelocity(velocity);
			DebugOut(L"2 enemy lai gap nhauuuuuuuuuuu \n");
		}
	}*/
	
}
