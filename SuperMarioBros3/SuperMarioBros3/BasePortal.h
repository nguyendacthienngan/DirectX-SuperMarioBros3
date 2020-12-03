#pragma once
#include <string>
class CBasePortal
{
protected:
    std::string sceneID;
    int cameraID;
public:
    void SetSceneID(std::string id);
    std::string GetSceneID();
    void SetCameraID(int id);
    int GetCameraID();
};
