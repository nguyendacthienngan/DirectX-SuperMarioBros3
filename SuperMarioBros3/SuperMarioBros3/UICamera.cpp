#include "UICamera.h"

CUICamera::CUICamera()
{
}

CUICamera::CUICamera(int wid, int hei)
{
	hud = new CHUD();
}

void CUICamera::Update()
{
}

void CUICamera::Render()
{
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

CUICamera::~CUICamera()
{
}
