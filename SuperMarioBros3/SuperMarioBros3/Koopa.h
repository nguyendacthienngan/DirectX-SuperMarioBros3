#pragma once
#include "Enemy.h"
class CKoopa: public CEnemy
{
public:
	CKoopa();
	void Init();
	void LoadAnimation();
	void Update(DWORD dt, CCamera* cam) override;
};

