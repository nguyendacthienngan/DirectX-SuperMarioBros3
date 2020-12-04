#include "NodeMap.h"
#include "SceneManager.h"

CNodeMap::CNodeMap()
{
	nodeTag = NodeTag::Normal;
}

CNodeMap::CNodeMap(D3DXVECTOR2 size)
{
	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(size);
	box->SetGameObjectAttach(this);
	box->SetName("Portal");
	box->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(box);
	this->isEnabled = true;

	physiscBody->SetTrigger(true);
	physiscBody->SetDynamic(false);

	nodeTag = NodeTag::Normal;
}

RECT CNodeMap::DirectionMarioCanMove(D3DXVECTOR2 posMario)
{
	RECT direction = { 0, 0, 0, 0 };
	if (graph == NULL)
	{
		auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
		if (activeScene != NULL)
		{
			auto map = activeScene->GetMap();
			if (map != NULL)
				graph = map->GetTileMap()->GetGraph();
		}
	}
	if (graph != NULL)
	{
		for (auto nodeIndex : adjacencyNodes)
		{
			DebugOut(L"NODE INDEX %d \n", nodeIndex);
			auto node = graph->GetNodeByID(nodeIndex);
			if (node != NULL)
			{
				auto nodeGO = static_cast<CGameObject*>(node);
				if (nodeGO->GetPosition().x < posMario.x)
					direction.left = 1;
				if (nodeGO->GetPosition().x > posMario.x)
					direction.right = 1;
				if (nodeGO->GetPosition().y < posMario.y)
					direction.top = 1;
				if (nodeGO->GetPosition().y > posMario.y)
					direction.bottom = 1;
			}
		}
	}
	DebugOut(L" DIRECTION %d %d %d %d \n", direction.left, direction.top, direction.right, direction.bottom);
	return direction;
}
