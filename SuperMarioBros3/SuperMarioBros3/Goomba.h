#pragma once
#include "Enemy.h"
class CGoomba : public CEnemy
{
public:
	CGoomba();
	void Init() override;
	void LoadAnimation() override;
	void Update(DWORD dt, CCamera* cam) override;
};

