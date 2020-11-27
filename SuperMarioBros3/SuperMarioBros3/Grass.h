#pragma once
#include "GameObject.h"
class CGrass :
    public CGameObject
{

public:
    CGrass();
   void LoadAnimation() override;
};

