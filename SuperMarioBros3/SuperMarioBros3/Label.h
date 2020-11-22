#pragma once
#include "GameObject.h"
class CLabel :
    public CGameObject
{
    RectF pushDirection; // top: 1, right, left, bottom : 0 => đẩy hướng top
public:
    CLabel(D3DXVECTOR2 size);
    void SetPushDirection(RectF pushDirect);
    RectF GetPushDirection();
};

