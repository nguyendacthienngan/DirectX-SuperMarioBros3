#include "BasePortal.h"

using namespace std;

void CBasePortal::SetSceneID(std::string id)
{
	sceneID = id;
}

std::string CBasePortal::GetSceneID()
{
	return sceneID;
}

void CBasePortal::SetCameraID(int id)
{
	cameraID = id;
}

int CBasePortal::GetCameraID()
{
	return cameraID;
}