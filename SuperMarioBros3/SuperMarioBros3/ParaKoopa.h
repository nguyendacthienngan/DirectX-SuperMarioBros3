#pragma once
#include "Enemy.h"
#include "GreenKoopa.h"
class CParaKoopa : public CEnemy
{
	CGreenKoopa* koopa;
public:
	CParaKoopa();
	void Init() override;
	void LoadAnimation() override;
	void Render(CCamera* cam, int alpha);
};

