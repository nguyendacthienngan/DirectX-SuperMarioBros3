#pragma once
#include "GameObject.h"
class CPortal :
    public CGameObject
{
private:
    std::string sceneID;
    int cameraID;
public:
    CPortal();
    CPortal(D3DXVECTOR2 size);
    void Init(D3DXVECTOR2 size);
    void SetSceneID(std::string id);
    std::string GetSceneID();
    void SetCameraID(int id);
    int GetCameraID();
};

