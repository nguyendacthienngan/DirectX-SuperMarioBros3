#pragma once
#include "GameObject.h"
#include "BasePortal.h"
class CPortal : public CGameObject, public CBasePortal
{
public:
    CPortal();
    CPortal(D3DXVECTOR2 size);
    void Init(D3DXVECTOR2 size);
};