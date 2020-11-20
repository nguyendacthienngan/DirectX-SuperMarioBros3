#include "EffectObject.h"

CEffectObject::CEffectObject()
{
	tag = GameObjectTags::Effect;
}

void CEffectObject::SetStartPosition(D3DXVECTOR2 pos)
{
	this->transform.position = pos;
	this->startPosition = pos;
}

bool CEffectObject::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	return false;
}
