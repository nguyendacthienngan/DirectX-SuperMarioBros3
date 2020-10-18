#include "CollisionBox.h"


void CCollisionBox::SetGameObjectAttach(LPGameObject gO)
{
	this->gameObject = gO;
}

LPGameObject CCollisionBox::GetGameObjectAttach()
{
	return gameObject;
}

void CCollisionBox::SetSizeBox(D3DXVECTOR2 size)
{
	sizeBox = size;
}

D3DXVECTOR2 CCollisionBox::GetSizeBox()
{
	return sizeBox;
}

void CCollisionBox::SetPosition(D3DXVECTOR2 pos)
{
	this->localPosition = pos;
}

D3DXVECTOR2 CCollisionBox::GetPosition()
{
	return localPosition;
}

void CCollisionBox::SetDistance(D3DXVECTOR2 d)
{
	distance = d;
}

D3DXVECTOR2 CCollisionBox::GetDistance()
{
	return distance;
}

D3DXVECTOR2 CCollisionBox::GetWorldPosition()
{
	return gameObject->GetPosition() + localPosition; // localPosition?
}

bool CCollisionBox::IsEnabled()
{
	return gameObject != NULL;
}

RectF CCollisionBox::GetBoundingBox()
{
	// GetBoundingBox la lay cai box nam trong the gioi game 
	// Con boxSize la kich thuoc thoi k co toa do
	// 0.5:
	// La cai box cua t muon no nam giua game object , tu tam (x, y) toa ra chu k phai la trai tren => Dong bo voi sprite

	auto pos = GetWorldPosition();
	RectF r;
	r.left = pos.x - sizeBox.x * 0.5f;
	r.right = pos.x + sizeBox.x * 0.5f;
	r.top = pos.y - sizeBox.y * 0.5f;
	r.bottom = pos.y + sizeBox.y * 0.5f;

	return r;
}

void CCollisionBox::SetName(std::string n)
{
	name = n;
}

std::string CCollisionBox::GetName()
{
	return name;
}

void CCollisionBox::SetId(int i)
{
	id = i;
}

int CCollisionBox::GetID()
{
	return id;
}
