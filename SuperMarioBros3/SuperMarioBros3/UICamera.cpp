#include "UICamera.h"
#include "Game.h"
#include "TextureManager.h"
#include "Const.h"
#include "UICameraConst.h"

CUICamera::CUICamera()
{
}

CUICamera::CUICamera(int wid, int hei)
{
	hud = new CHUD();
    this->widthCam = wid;
    this->heightCam = hei;
}

void CUICamera::Update()
{
}

void CUICamera::Render()
{
	auto tex = CTextureManager::GetInstance()->GetTexture(TEXTURE_BLACK);
    D3DXVECTOR2 posInCam;
	posInCam.x = trunc(hud->GetPosition().x - this->posCam.x);
	posInCam.y = trunc(hud->GetPosition().y - this->posCam.y) + BLACK_RECTANGLE_HEIGHT;
    float surfaceWidth = surfaceRect.right - surfaceRect.left;
    float surfaceHeight = surfaceRect.bottom - surfaceRect.top;
	CGame::GetInstance()->Draw(posInCam, D3DXVECTOR2(surfaceWidth * 0.5f, surfaceHeight * 0.5f), tex, surfaceRect, D3DCOLOR_XRGB(0,0,0));

    if (hud != NULL)
    {
        hud->Render(this);
    }
}

CHUD* CUICamera::GetHUD()
{
    return hud;
}

void CUICamera::SetHUD(CHUD* hud)
{
    this->hud = hud;
}

void CUICamera::SetPositionCam(D3DXVECTOR2 pos)
{
    CCamera::SetPositionCam(pos);
    surfaceRect.left = 0;
    surfaceRect.top = 0;
    surfaceRect.right = surfaceRect.left + widthCam + BLACK_RECTANGLE_WIDTH;
    surfaceRect.bottom = surfaceRect.top + heightCam;
}

CUICamera::~CUICamera()
{

}
