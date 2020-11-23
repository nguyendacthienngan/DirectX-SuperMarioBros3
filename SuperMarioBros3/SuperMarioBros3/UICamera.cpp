#include "UICamera.h"
#include "Game.h"

CUICamera::CUICamera()
{
}

CUICamera::CUICamera(int wid, int hei)
{
	hud = new CHUD();
    auto d3ddev = CGame::GetInstance()->GetDirect3DDevice();
    auto backbuffer = CGame::GetInstance()->GetBackBuffer();
    //auto surface = CGame::GetInstance()->GetSurface();
    LPDIRECT3DSURFACE9 surface = NULL;
    /*d3ddev->Clear(
        0,
        NULL,
        D3DCLEAR_TARGET,
        D3DCOLOR_XRGB(255, 0, 0),
        1.0f,
        0
    );*/
   /* d3ddev->GetBackBuffer(
        0,
        0,
        D3DBACKBUFFER_TYPE_MONO,
        &backbuffer
    );*/
    d3ddev->CreateOffscreenPlainSurface(
        600, // width
        600, // height
        D3DFMT_X8R8G8B8,
        D3DPOOL_DEFAULT,
        &surface,
        NULL
    );
    CGame::GetInstance()->SetSurface(surface);
}

void CUICamera::Update()
{
}

void CUICamera::Render()
{
    auto d3ddev = CGame::GetInstance()->GetDirect3DDevice();
    auto backbuffer = CGame::GetInstance()->GetBackBuffer();
    auto surface = CGame::GetInstance()->GetSurface();
    RECT rect = { 10, 10, 650, 650 };

    d3ddev->ColorFill(
        surface,
        &rect,
        D3DCOLOR_XRGB(255, 0, 0)
    );
    d3ddev->StretchRect(
        surface,
        NULL,
        backbuffer,
        &rect,
        D3DTEXF_NONE
    );
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
  /*  surfaceRect.left = 0;
    surfaceRect.top = 0;
    surfaceRect.right = surfaceRect.left + widthCam;
    surfaceRect.bottom = surfaceRect.top + heightCam;*/
}

CUICamera::~CUICamera()
{

}
