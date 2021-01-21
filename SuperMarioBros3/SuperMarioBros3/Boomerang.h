#pragma once
#include "GameObject.h"
class CBoomerang :
    public CGameObject
{
private:
    CObjectPool* pool;
    CGameObject* boomerangBrother;
    int attackState;
    D3DXVECTOR2 goalPosition;
    D3DXVECTOR2 startPosition;
public:
    CBoomerang();
    void LoadAnimation() override;
    void Render(CCamera* cam, int alpha) override;
    virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
    void LinkToPool(CObjectPool* pool);
    void SetGoalPosition(D3DXVECTOR2 goal);
    void SetStartPosition(D3DXVECTOR2 start);
    void SetBoomerangBrother(CGameObject* gO);
    bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
    void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;
    void SetAttackState(int attackState);
    int GetAttackState();
};

