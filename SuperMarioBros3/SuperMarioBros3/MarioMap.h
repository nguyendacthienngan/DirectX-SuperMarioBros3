#pragma once
#include "GameObject.h"
#include "Graph.h"
class CGraph;
class CMarioMap: public CGameObject
{
private:
	RECT direction;
	std::string sceneID;
	CGraph* graph;
	CNode* currentNode;
public:
	CMarioMap();
	void LoadAnimation();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha = 255) override;
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox);
	bool SwitchScene();
};

