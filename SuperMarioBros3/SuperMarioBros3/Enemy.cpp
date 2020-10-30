#include "Enemy.h"

CEnemy::CEnemy()
{
	this->SetTag(GameObjectTags::Enemy);
	this->SetScale(D3DXVECTOR2(1.0f, 1.0f));

}

void CEnemy::SetEnemyType(std::string eT)
{
	enemyType = eT;
}

std::string CEnemy::GetEnemyType()
{
	return enemyType;
}

void CEnemy::SetStartPosition(D3DXVECTOR2 sT)
{
	this->startPosition = sT;
}

D3DXVECTOR2 CEnemy::GetStartPosition()
{
	return startPosition;
}
