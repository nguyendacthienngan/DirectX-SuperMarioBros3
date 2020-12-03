#pragma once
#include "BasePortal.h"
#include "GameObject.h"
class CSceneGate : public CBasePortal, public CGameObject
{
    RectF pushDirection; // top: 1, right, left, bottom : 0 => đẩy hướng top
public:
    CSceneGate();
    CSceneGate(D3DXVECTOR2 size);
    void LoadAnimation();
    void Render(CCamera* cam, int alpha = 255);
    void Init(D3DXVECTOR2 size);
};