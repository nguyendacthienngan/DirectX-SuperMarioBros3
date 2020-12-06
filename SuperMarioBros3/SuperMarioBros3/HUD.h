#pragma once
#include "PMeter.h"
#include "CardGift.h"
class CHUD
{
private:
    CPMeter* pMeter;
    CCardGift* card1;
    CCardGift* card2;
    CCardGift* card3;
    CSprite* hudSprite;
    D3DXVECTOR2 pos;
public:
    CHUD(D3DXVECTOR2 hudPos);
    void LoadSprite();
    void Update();
    void Render();
    void SetPosition(D3DXVECTOR2 pos);
    D3DXVECTOR2 GetPosition();
    void SetPMeter(CPMeter* pMeter);
    CPMeter* GetPMeter();
    void SetCard(int index, std::string state);
    CCardGift* GetCard(int index);
};

