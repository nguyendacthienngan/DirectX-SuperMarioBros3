#pragma once
#include "Portal.h"
class CSceneGate :
    public CPortal
{
public:
    CSceneGate();
    CSceneGate(D3DXVECTOR2 size);
    void LoadAnimation();
    void Render(CCamera* cam, int alpha = 255);
};

