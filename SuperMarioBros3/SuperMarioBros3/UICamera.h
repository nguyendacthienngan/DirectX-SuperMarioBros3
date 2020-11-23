#pragma once
#include "Camera.h"
#include "HUD.h"
class CHUD;
class CCamera;
class CUICamera :  public CCamera
{
    CHUD* hud;
public:
    CUICamera();
    CUICamera(int wid, int hei);
    void Update();
    void Render();
    CHUD* GetHUD();
    void SetHUD(CHUD* hud);
    ~CUICamera();
};

