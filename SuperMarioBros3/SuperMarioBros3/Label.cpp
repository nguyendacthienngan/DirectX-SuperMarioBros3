#include "Label.h"

CLabel::CLabel(D3DXVECTOR2 size)
{
	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(size);
	box->SetGameObjectAttach(this);
	box->SetName("Label");
	box->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(box);
	this->isEnabled = true;

	physiscBody->SetDynamic(false);
}

void CLabel::SetPushDirection(RectF pushDirect)
{
	pushDirection = pushDirect;
}

RectF CLabel::GetPushDirection()
{
	return pushDirection;
}
