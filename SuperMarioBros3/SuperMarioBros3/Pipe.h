#pragma once
#include "GameObject.h"
#include "PipeConst.h"
class CPipe :
    public CGameObject
{
    CSprite* pipeSprite;
    PipeDirection pipeDirection;
    RECT  headRect[2];
    RECT bodyRect[2];
    D3DXVECTOR2 pipeSize;
public:
    CPipe(D3DXVECTOR2 size);
    void Render(CCamera* cam, int alpha = 255) override;
    void SetPipeSprite(CSprite* pipeSprite);
    void SetHeadRect(RECT headRect1, RECT headRect2);
    void SetBodyRect(RECT bodyRect1, RECT bodyRect2);
    void SetDirection(PipeDirection pipeDirection);
    bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
};

