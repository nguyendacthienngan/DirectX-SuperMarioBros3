#pragma once
#include "EffectObject.h"
class CCoinEffect : public CEffectObject
{
public:
	CCoinEffect();
	void LoadAnimation();
	virtual void Update(DWORD dt, CCamera* cam);
};

