#pragma once
#include "GameObject.h"
class CEffectObject: public CGameObject
{
public:
	CEffectObject();
	void SetStartPosition(D3DXVECTOR2 pos);
};

