#pragma once
#include "Enemy.h"
#include "Koopa.h"
class ParaKoopa : public CEnemy
{
	CKoopa* koopa;
public:
	ParaKoopa();
	void Init() override;
	void LoadAnimation() override;
};

