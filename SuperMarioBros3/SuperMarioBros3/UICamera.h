#pragma once
#include "Camera.h"
#include "HUD.h"
#include "Font.h"
#include "GoalEffects.h"
class CHUD;
class CCamera;
class CUICamera :  public CCamera
{
    CHUD* hud;
    RECT surfaceRect;
    std::vector<CFont*> texts;
    bool disableBlackTexture;
    std::string goalState;
    bool isGoalRoulette;
    DWORD goalTimer, waitingTimer;
    bool fontResultDisplayed;
    CSprite* cardResultInFont;
    CCardGift* giftInFont;
public:
    CUICamera();
    CUICamera(int wid, int hei, D3DXVECTOR2 hudPos);
    void Update();
    void Render();

    CHUD* GetHUD();
    void SetHUD(CHUD* hud);

    void AddText(CFont* text);

    void SetPositionCam(D3DXVECTOR2 pos) override;
    void SetDisableBlackTexture(bool disT);

    void OnGoalRoulette(std::string cardState);
    void GoalRouletteProcess();
    void FontResult();

    void EmptyTexts();
    ~CUICamera();
};

