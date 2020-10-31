#pragma once
#include "GameObject.h"
#include "EnemyConst.h"
class CEnemy : public CGameObject
{
protected:
	D3DXVECTOR2 startPosition; // vị trí mà sau khi di chuyển nó quay trở lại
	float distanceToMove;
	std::string enemyType;
	DWORD startDeadTime;
public:
	CEnemy();
	void SetEnemyType(std::string eT);
	std::string GetEnemyType();

	void SetStartPosition(D3DXVECTOR2 sT);
	D3DXVECTOR2 GetStartPosition();

	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;

};
