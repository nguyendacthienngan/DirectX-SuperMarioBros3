#include "CollisionBox.h"
#include "Ultis.h"
#include "Game.h"
#include "TextureManager.h"
#include "GraphicConst.h"

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

void CCollisionBox::RenderBoundingBox()
{
	RectF bbRectF = GetBoundingBox();
	RECT bbRect;
	bbRect.left = 0;
	bbRect.top = 0;
	bbRect.right = sizeBox.x;
	bbRect.bottom = sizeBox.y;

	
	//bbRect.left += 20;

	// Đang đi cái rớt cái bụp => Tọa độ đang lệch??
	//OutputDebugString(ToLPCWSTR("BBName: " + name));
	//DebugOut(L"(left,top,right,bottom): (%f,%f,%f,%f) \n", bbRectF.left, bbRectF.top, bbRectF.right, bbRectF.bottom);
	//DebugOut(L"(left,top,right,bottom): (%d,%d,%d,%d) \n", bbRect.left, bbRect.top, bbRect.right, bbRect.bottom);
	auto tex = CTextureManager::GetInstance()->GetTexture(BOUNDINGBOX_TEXTURE);
	auto pos = GetWorldPosition();
	//DebugOut(L"(x,y): (%f,%f) \n", pos.x, pos.y);

	CGame::GetInstance()->Draw(pos, D3DXVECTOR2(sizeBox.x/2, sizeBox.y/2), tex, bbRect, 32);
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
	//return gameObject->GetPosition(); 
}

bool CCollisionBox::IsEnabled()
{
	return gameObject != NULL;
}

RectF CCollisionBox::GetBoundingBox()
{
	// GetBoundingBox la lay cai box nam trong the gioi game 
	// Con boxSize la kich thuoc thoi k co toa do
	
	auto pos = GetWorldPosition();
	RectF r;
	r.left = pos.x;
	r.right = pos.x + sizeBox.x;
	r.top =  pos.y ;
	r.bottom = pos.y + sizeBox.y ;

	/*if (name == "Mario")
		DebugOut(L"BB Mario: RECT (l,t,r,bt) : (%f,%f,%f,%f) \n", r.left, r.top, r.right, r.bottom);*/

	/*r.left = pos.x - sizeBox.x * 0.5f;
	r.right = pos.x + sizeBox.x * 0.5f;
	r.top = pos.y - sizeBox.y * 0.5f;
	r.bottom = pos.y + sizeBox.y * 0.5f;*/
	// 0.5:
	// La cai box cua t muon no nam giua game object , tu tam (x, y) toa ra chu k phai la trai tren => Dong bo voi sprite

	//OutputDebugString(ToLPCWSTR("BB: " + name + "\n")); // Bach mun debug out cai gi tên
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
