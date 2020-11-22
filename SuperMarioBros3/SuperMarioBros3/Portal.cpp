#include "Portal.h"

CPortal::CPortal(D3DXVECTOR2 size)
{
	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(size);
	box->SetGameObjectAttach(this);
	box->SetName("Portal");
	box->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(box);
	this->isEnabled = true;

	physiscBody->SetDynamic(false);
}

void CPortal::SetSceneID(int id)
{
	sceneID = id;
}

int CPortal::GetSceneID()
{
	return sceneID;
}

void CPortal::SetCameraID(int id)
{
	cameraID = id;
}

int CPortal::GetCameraID()
{
	return cameraID;
}
