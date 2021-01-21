#pragma once
#include "Enemy.h"
class CBoomerangBrother :
    public CEnemy
{
private:
    CObjectPool boomerangs;
    int moveState;
    bool canAttack;
    bool isAttack;
    bool canThrowSecondBoomerang;
    int idleTimer, attackTimer;
    int attackTime;
    int countThrowTimes;
    CGameObject* onHoldObject;
    bool isHoldBoomerang;
public:
    CBoomerangBrother();
    void LoadAnimation() override;
    void Render(CCamera* cam, int alpha) override;
    virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
    CObjectPool GetObjectPool();
    void OnAttack(D3DXVECTOR2 normal);
    void OnMovingForward(D3DXVECTOR2 normal);
    void OnMovingBackwards(D3DXVECTOR2 normal);
    void OnHoldBoomerang(D3DXVECTOR2 normal);
};

