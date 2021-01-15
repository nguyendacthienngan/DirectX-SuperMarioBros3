#pragma once
#include "Enemy.h"
#include "Koopa.h"
class CParaKoopa : public CEnemy
{
	CKoopa* koopa;
public:
	CParaKoopa();
	void Init() override;
	void Render(CCamera* cam, int alpha);
	void ChangeToKoopa();
	CKoopa* GetKoopa();
	void SetKoopa(CKoopa* koopa);
	bool IsOnGround();
	void OnDamaged(CGameObject* otherGO) override;

};

