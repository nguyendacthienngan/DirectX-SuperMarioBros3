#pragma once
#include "EffectObject.h"
class CBrickEffect : public CEffectObject
{
	CObjectPool* pool;
public:
	CBrickEffect();
	void LoadAnimation();
	void LinkToPool(CObjectPool* pool);
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
};

