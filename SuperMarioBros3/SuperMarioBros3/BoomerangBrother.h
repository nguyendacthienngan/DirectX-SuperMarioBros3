#pragma once
#include "Enemy.h"
class CBoomerangBrother :
    public CEnemy
{
private:
    CObjectPool boomerangs;
public:
    CBoomerangBrother();
    void LoadAnimation() override;
    void Render(CCamera* cam, int alpha) override;
    virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
    CObjectPool GetObjectPool();
};

