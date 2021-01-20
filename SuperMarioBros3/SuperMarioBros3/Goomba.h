#pragma once
#include "Enemy.h"
#include "GoombaConst.h"
class CGoomba : public CEnemy
{
protected:
	GoombaState currentPhysicsState;
	D3DXVECTOR2 beforeHitPosition;
	bool isJumpMaxHeight;
public:
	CGoomba();
	void Init() override;
	void LoadAnimation() override;
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha = 255) override;
	virtual void OnDie();
};

