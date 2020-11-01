#pragma once
#include "GameObject.h"
class CHoldable
{
protected:
	CGameObject* holder;
	D3DXVECTOR2 normal;
public:
	virtual void SetHoldablePosition(D3DXVECTOR2 pos) = 0;
	virtual void Release();
	virtual D3DXVECTOR2 GetHoldableCollisionBox() = 0;

	virtual CGameObject* GetHolder();
	virtual void SetHolder(CGameObject* holder);
	virtual D3DXVECTOR2 GetHoldableNormal();
	virtual void SetHoldableNormal(D3DXVECTOR2 normal);
	virtual bool IsHolding();
};

