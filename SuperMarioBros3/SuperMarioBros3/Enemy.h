#pragma once
#include "GameObject.h"
#include "EnemyConst.h"
#include "HitEffects.h"

class CEnemy : public CGameObject
{
protected:
	D3DXVECTOR2 startPosition; // vị trí mà sau khi di chuyển nó quay trở lại
	std::string enemyType;
	EnemyTag enemyTag;
	DWORD startDeadTime;
	bool isDead, isHeadShot, isHeadShotByFireBall;
	int countDeadCallback;
	CHitEffects* hitFX;
	bool isOnGround;

public:
	CEnemy();
	void SetEnemyType(std::string eT);
	std::string GetEnemyType();

	void SetEnemyTag(EnemyTag eT);
	EnemyTag GetEnemyTag();

	void SetStartPosition(D3DXVECTOR2 sT);
	D3DXVECTOR2 GetStartPosition();

	virtual void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents) override;
	virtual void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
	void SetHitFX(CHitEffects* hitFX);
	void SetIsHeadShot(bool isHeadShot);
	void SetIsHeadShotByFireball(bool isHeadShotByFireball);
};
