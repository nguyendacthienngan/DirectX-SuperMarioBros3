#pragma once
#include "Camera.h"
#include "HUD.h"
#include "Font.h"
class CHUD;
class CCamera;
class CUICamera :  public CCamera
{
    CHUD* hud;
    RECT surfaceRect;
    CFont* font;
public:
    CUICamera();
    CUICamera(int wid, int hei, D3DXVECTOR2 hudPos);
    void Update();
    void Render();

    CHUD* GetHUD();
    void SetHUD(CHUD* hud);

    void SetPositionCam(D3DXVECTOR2 pos) override;
    ~CUICamera();
};

