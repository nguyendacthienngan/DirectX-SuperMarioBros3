#pragma once
#include "Camera.h"
#include "HUD.h"
class CHUD;
class CCamera;
class CUICamera :  public CCamera
{
    CHUD* hud;
    // LPDIRECT3DSURFACE9 surface = NULL;
    RECT surfaceRect;
public:
    CUICamera();
    CUICamera(int wid, int hei);
    void Update();
    void Render();

    CHUD* GetHUD();
    void SetHUD(CHUD* hud);

    void SetPositionCam(D3DXVECTOR2 pos) override;
    ~CUICamera();
};

