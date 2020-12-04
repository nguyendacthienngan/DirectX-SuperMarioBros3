#include "SceneGate.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "SceneManager.h"

CSceneGate::CSceneGate()
{
	nodeTag = NodeTag::Portal;
}

CSceneGate::CSceneGate(D3DXVECTOR2 size)
{
	Init(size);
	LoadAnimation();
	SetState("SCENE-1");
	nodeTag = NodeTag::Portal;
}

void CSceneGate::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("SCENE-1", animationManager->Get("ani-gate-1"));
	AddAnimation("SCENE-2", animationManager->Get("ani-gate-2"));
	AddAnimation("SCENE-3", animationManager->Get("ani-gate-3"));
	AddAnimation("SCENE-4", animationManager->Get("ani-gate-4"));
	AddAnimation("SCENE-5", animationManager->Get("ani-gate-5"));
	AddAnimation("SCENE-6", animationManager->Get("ani-gate-6"));

	AddAnimation("MUSHROOM-GATE", animationManager->Get("ani-musroom-gate"));
	AddAnimation("DOMED-GATE", animationManager->Get("ani-domed-gate"));
	AddAnimation("SPADE", animationManager->Get("ani-spade"));
	AddAnimation("CASTLE", animationManager->Get("ani-castle"));
}

void CSceneGate::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}

void CSceneGate::Init(D3DXVECTOR2 size)
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
	physiscBody->SetTrigger(true);

	sceneID = "";
	cameraID = -1;
}

RECT CSceneGate::DirectionMarioCanMove(D3DXVECTOR2 posMario)
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
	return direction;
}
