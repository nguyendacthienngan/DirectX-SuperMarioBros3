#pragma once
#include "EffectObject.h"
class CCoinEffect : public CEffectObject
{
	D3DXVECTOR2 startPosition;
public:
	CCoinEffect();
	void LoadAnimation();
	virtual void Update(DWORD dt, CCamera* cam);
	void SetStartPosition(D3DXVECTOR2 sP);
};

