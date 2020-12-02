#include "Portal.h"
#include "Ultis.h"

CPortal::CPortal()
{
}

CPortal::CPortal(D3DXVECTOR2 size)
{
	Init(size);
}

void CPortal::Init(D3DXVECTOR2 size)
{
	this->SetTag(GameObjectTags::Portal);

	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(size);
	box->SetGameObjectAttach(this);
	box->SetName("Portal");
	box->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(box);
	this->isEnabled = true;

	physiscBody->SetDynamic(false);

	sceneID = "";
	cameraID = -1;
}

void CPortal::SetSceneID(std::string id)
{
	sceneID = id;
}

std::string CPortal::GetSceneID()
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
