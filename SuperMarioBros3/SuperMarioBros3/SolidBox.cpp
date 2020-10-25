#include "SolidBox.h"
#include "Ultis.h"
#include "TextureManager.h"
#include "Const.h"
#include "Game.h"

CSolidBox::CSolidBox()
{
	Init();
}



void CSolidBox::Init()
{
	CCollisionBox* box = new CCollisionBox();
	box->SetGameObjectAttach(this);
	this->collisionBoxs->push_back(box);
	this->physiscBody->SetDynamic(false);
	this->SetTag(GameObjectTags::Solid);
}

void CSolidBox::Render(CCamera* cam)
{
	auto sizeBox = collisionBoxs->at(0)->GetSizeBox();
	auto name = collisionBoxs->at(0)->GetName();
	auto pos = collisionBoxs->at(0)->GetWorldPosition();
	auto tex = CTextureManager::GetInstance()->GetTexture(TEXTURE_BBOX);
	RECT bbRect;
	bbRect.left = 0;
	bbRect.top = 0;
	bbRect.right = sizeBox.x;
	bbRect.bottom = sizeBox.y;

	D3DXVECTOR2 posInCam, camPos;
	camPos = cam->GetPositionCam();
	posInCam.x = trunc(pos.x - camPos.x );
	posInCam.y = trunc(pos.y - camPos.y - 18);

	CGame::GetInstance()->Draw(posInCam, tex, bbRect, 255);
}
