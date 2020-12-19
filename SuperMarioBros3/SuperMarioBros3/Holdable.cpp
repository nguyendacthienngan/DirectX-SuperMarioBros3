#include "Holdable.h"

void CHoldable::Release(bool isFromHigher)
{
	this->holder = NULL;
}

CGameObject* CHoldable::GetHolder()
{
	return holder;
}

void CHoldable::SetHolder(CGameObject* holder)
{
	this->holder = holder;
}

D3DXVECTOR2 CHoldable::GetHoldableNormal()
{
	return normal;
}

void CHoldable::SetHoldableNormal(D3DXVECTOR2 normal)
{
	this->normal = normal;
}

bool CHoldable::IsHolding()
{
	return holder != NULL;
}

