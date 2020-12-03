#pragma once
#include "GameObject.h"
class CNodeMap :
    public CGameObject
{
public:
    CNodeMap();
    CNodeMap(D3DXVECTOR2 size);
    RECT DirectionMarioCanMove(D3DXVECTOR2 posMario);
};

