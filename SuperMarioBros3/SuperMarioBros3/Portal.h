#pragma once
#include "GameObject.h"
class CPortal :
    public CGameObject
{
private:
    int sceneID;
    int cameraID;
public:
    CPortal(D3DXVECTOR2 size);
    void SetSceneID(int id);
    int GetSceneID();
    void SetCameraID(int id);
    int GetCameraID();
};

