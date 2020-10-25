#include "CollisionBox.h"
#include "Ultis.h"
#include "Game.h"
#include "TextureManager.h"
#include "GraphicConst.h"
#include "Const.h"

CCollisionBox::CCollisionBox()
{
	distance.x = 0;
	distance.y = 0;
	localPosition.x = 0;
	localPosition.y = 0;
	name = "";
	id = -1;
	gameObject = NULL;
	sizeBox.x = 1;
	sizeBox.y = 1;
}

void CCollisionBox::Render(CCamera* camera, int distance)
{
	auto pos = GetWorldPosition();
	auto tex = CTextureManager::GetInstance()->GetTexture(TEXTURE_BBOX);
	RECT bbRect;
	bbRect.left = 0;
	bbRect.top = 0;
	bbRect.right = sizeBox.x;
	bbRect.bottom = sizeBox.y;

	D3DXVECTOR2 posInCam, camPos;
	camPos = camera->GetPositionCam();
	posInCam.x = trunc(pos.x - camPos.x + distance);
	posInCam.y = trunc(pos.y - camPos.y + distance);

	CGame::GetInstance()->Draw(posInCam, tex, bbRect, 32);
}

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
	return gameObject->GetPosition() + localPosition;
}

bool CCollisionBox::IsEnabled()
{
	return gameObject != NULL;
}

RectF CCollisionBox::GetBoundingBox()
{
	// GetBoundingBox la lay cai box nam trong the gioi game 
	// Con boxSize la kich thuoc thoi k co toa do
	int pointCenterX = 0;
	auto tag = gameObject->GetTag();
	/*if (tag == GameObjectTags::RaccoonMario)
		pointCenterX = gameObject->GetAnimationByState(gameObject->GetCu);*/
	auto pos = GetWorldPosition();
	RectF r;
	r.left = pos.x;
	r.right = pos.x + sizeBox.x;
	r.top =  pos.y ;
	r.bottom = pos.y + sizeBox.y ;
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
