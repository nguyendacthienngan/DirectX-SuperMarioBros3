#include "UICamera.h"
#include "Game.h"
#include "TextureManager.h"
#include "Const.h"
#include "UICameraConst.h"
#include "WorldMap1.h"
#include "SceneManager.h"

CUICamera::CUICamera()
{
}

CUICamera::CUICamera(int wid, int hei, D3DXVECTOR2 hudPos)
{
	hud = new CHUD(hudPos);
    menu = new CMenu();
    this->widthCam = wid;
    this->heightCam = hei;
    disableBlackTexture = false;
    isGoalRoulette = false;
    waitingTimer = 0;
    goalTimer = 0;
    fontResultDisplayed = false;
    isIntro = false;
    giftInFont = NULL;
}

void CUICamera::Update()
{
    if (CGame::GetTimeScale() == 0)
        return;
    hud->Update();
    GoalRouletteProcess();
    IntroProcess();
}

void CUICamera::Render()
{
    if (disableBlackTexture == false)
    {
        auto tex = CTextureManager::GetInstance()->GetTexture(TEXTURE_BLACK);
        D3DXVECTOR2 posInCam;
        posInCam.x = trunc(hud->GetPosition().x - this->posCam.x);
        posInCam.y = trunc(hud->GetPosition().y - this->posCam.y) + BLACK_RECTANGLE_HEIGHT;
        float surfaceWidth = surfaceRect.right - surfaceRect.left;
        float surfaceHeight = surfaceRect.bottom - surfaceRect.top;
        CGame::GetInstance()->Draw(posInCam, D3DXVECTOR2(surfaceWidth * 0.5f, surfaceHeight * 0.5f), tex, surfaceRect, D3DCOLOR_XRGB(0, 0, 0));
    }
	if (hud != NULL)
    {
       hud->Render();
    }
    if (texts.size() > 0)
    {
        for (auto text : texts)
            text->Render();
    }
    if (giftInFont != NULL)
        giftInFont->Render();

    if (menu != NULL && isIntro == true)
    {
        menu->Render();
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

void CUICamera::AddText(CFont* text)
{
    if (text == NULL) return;
    texts.push_back(text);
}

void CUICamera::SetPositionCam(D3DXVECTOR2 pos)
{
    CCamera::SetPositionCam(pos);
    surfaceRect.left = 0;
    surfaceRect.top = 0;
    surfaceRect.right = surfaceRect.left + widthCam + BLACK_RECTANGLE_WIDTH;
    surfaceRect.bottom = surfaceRect.top + heightCam;
}

void CUICamera::SetDisableBlackTexture(bool disT)
{
    this->disableBlackTexture = disT;
}

void CUICamera::OnGoalRoulette(std::string cardState)
{
    goalState = cardState;
    isGoalRoulette = true;
}

void CUICamera::GoalRouletteProcess()
{
    if (isGoalRoulette == false)
        return;
    waitingTimer += CGame::GetInstance()->GetDeltaTime() * CGame::GetTimeScale();
    if (waitingTimer > WAITING_TIME)
    {
        goalTimer += CGame::GetInstance()->GetDeltaTime();
        if (goalTimer > GOAL_ROULETTE_TIME && this->GetHUD()->GetTimer()->GetTimerState() == 0) // miliseconds
        {
            // Chuyển scene
            auto sceneManager = CSceneManager::GetInstance();
            CWorldMap1* wolrdMap1 = new CWorldMap1();
            sceneManager->SwitchScene(wolrdMap1);
            isGoalRoulette = false;
            giftInFont = NULL;
            goalTimer = 0;
            waitingTimer = 0;
            EmptyTexts();
            return;
        }
        // Hiển thị font và card
        if (fontResultDisplayed == false)
            FontResult();
        //Reset game timer to zero
        if (this->GetHUD()->GetTimer()->GetTimerState() == 1)
            this->GetHUD()->GetTimer()->ResetToZero();
        // Hiển thị card bên HUD 
        this->GetHUD()->SetCard(1, goalState);
    }
}

void CUICamera::FontResult()
{
    CFont* font = new CFont();
    font->SetPosition(D3DXVECTOR2(250,150));
    font->SetCurrentText("COURSE CLEAR !");
    AddText(font);
    CFont* font1 = new CFont();
    font1->SetPosition(D3DXVECTOR2(200, 200));
    font1->SetCurrentText("YOU GOT A CARD");
    AddText(font1);
    giftInFont = new CCardGift();
    giftInFont->SetState(goalState);
    giftInFont->SetPosition(D3DXVECTOR2(580, 220));
    fontResultDisplayed = true;

}

void CUICamera::IntroProcess()
{
    if (isIntro == false || menu == NULL)
        return;
    hud->Enable(false);
    menu->Update();
}

void CUICamera::SetIntro(bool isIntro)
{
    this->isIntro = isIntro;
}

void CUICamera::EmptyTexts()
{
    for (auto tex : texts)
    {
        delete tex;
        tex = NULL;
    }
    texts.clear();
}

CUICamera::~CUICamera()
{

}
