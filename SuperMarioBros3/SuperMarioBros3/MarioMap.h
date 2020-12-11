#pragma once
#include "GameObject.h"
#include "Graph.h"
class CGraph;
class CMarioMap: public CGameObject
{
private:
	RECT canGoDirection;
	RECT currentDirection;
	std::string sceneID;
	CGraph* graph;
	CNode* currentNode;
	int moveState; // 0: Chưa di chuyển, 1: Di chuyển, chưa tới đích, 2: Tới node tiếp theo (Lúc đó gán lại = 0 và cho dừng)
	bool canEnterScene;
public:
	CMarioMap();
	void LoadAnimation();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha = 255) override;
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox);
	bool SwitchScene();
	void OnKeyDown(int KeyCode);
};

