#pragma once
#include "PMeter.h"
class CHUD
{
private:
    CPMeter* pMeter;
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
};

