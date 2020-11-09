#pragma once
#include "CollisionBox.h"
#include "Mario.h"
class CMarioCollisionBox : public CCollisionBox
{
private:
	CMario* mario;
public: 
	void CollisionHandle(DWORD dt, std::vector<CollisionEvent*>& collisions, LPPhysicsBody phyBody, D3DXVECTOR2 vel, int mintx, int minty, float nx, float ny) override;
	void SetGameObjectAttach(LPGameObject gO) override;
};

