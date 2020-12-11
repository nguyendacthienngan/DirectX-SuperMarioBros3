#include "MarioMap.h"
#include "AnimationManager.h"
#include "MarioMapConst.h"
#include "KeyboardManager.h"
#include "Portal.h"
#include "SceneManager.h"
#include "Scene1.h"
#include "NodeMap.h"
#include "SceneGate.h"
CMarioMap::CMarioMap()
{
	tag = GameObjectTags::MarioMap;
	LoadAnimation();
	SetState(SMALL_MARIO_MAP_STATE);
	isEnabled = true;
	canEnterScene = false;
	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(0.0f);
	physiscBody->SetTrigger(true);

	canGoDirection = { 0, 0, 0, 0 };
	currentDirection = { 0, 0, 0, 0 };

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(D3DXVECTOR2(14*3,16*3));
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Small-Mario-Map"); // SmallMario
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	//SetRelativePositionOnScreen(D3DXVECTOR2(0.0f, SUPER_MARIO_BBOX.y / 2 - SMALL_MARIO_BBOX.y / 2));
	this->collisionBoxs->push_back(collisionBox);

	sceneID = "";
	graph = NULL;
	currentNode = NULL;
	moveState = 0; 
}

void CMarioMap::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(SMALL_MARIO_MAP_STATE, animationManager->Get("ani-small-mario-map"));
	AddAnimation(BIG_MARIO_MAP_STATE, animationManager->Get("ani-big-mario-map"));
	AddAnimation(RACCOON_MARIO_MAP_STATE, animationManager->Get("ani-raccoon-mario-map"));
	AddAnimation(FIRE_MARIO_MAP_STATE, animationManager->Get("ani-fire-mario-map"));
}

void CMarioMap::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	if (SwitchScene() == true)
		return;
	if (graph == NULL)
	{
		auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
		auto mapGraph = activeScene->GetMap()->GetTileMap()->GetGraph();
		if (mapGraph != NULL)
		{
			graph = mapGraph;
			auto nodes = mapGraph->GetListNodes();
			if (nodes.size() > 0)
				currentNode = nodes.at(0);
		}
	}
	auto keyboard = CKeyboardManager::GetInstance();
	switch (moveState)
	{
		case 0:
		{
			if (currentNode != NULL)
			{
				if (currentNode->GetNodeTag() == NodeTag::Normal)
				{
					auto normalNode = static_cast<CNodeMap*>(currentNode);
					canGoDirection = normalNode->DirectionMarioCanMove(normalNode->GetPosition());
				}
				if (currentNode->GetNodeTag() == NodeTag::Portal)
				{
					auto portalNode = static_cast<CSceneGate*>(currentNode);
					canGoDirection = portalNode->DirectionMarioCanMove(portalNode->GetPosition());
				}
			}
			if (keyboard->GetKeyStateDown(DIK_RIGHT) && canGoDirection.right == 1)
			{
				moveState = 1;
				currentDirection.right = 1;
			}
			if (keyboard->GetKeyStateDown(DIK_LEFT) && canGoDirection.left == 1)
			{
				moveState = 1;
				currentDirection.left = 1;
			}
			if (keyboard->GetKeyStateDown(DIK_UP) && canGoDirection.top == 1)
			{
				moveState = 1;
				currentDirection.top = 1;
			}
			if (keyboard->GetKeyStateDown(DIK_DOWN) && canGoDirection.bottom == 1)
			{
				moveState = 1;
				currentDirection.bottom = 1;
			}
			break;
		}
		case 1:
		{
			if (currentDirection.right == 1)
				transform.position.x += MARIO_MAP_VELOCITY.x * dt;
			if (currentDirection.left == 1)
				transform.position.x -= MARIO_MAP_VELOCITY.x * dt;
			if (currentDirection.top == 1)
				transform.position.y -= MARIO_MAP_VELOCITY.y * dt;
			if (currentDirection.bottom == 1)
				transform.position.y += MARIO_MAP_VELOCITY.y * dt;
			break;
		}
		case 2:
		{
			auto currNode = static_cast<CGameObject*>(currentNode);
			if (currentDirection.right == 1)
			{
				transform.position.x += MARIO_MAP_VELOCITY.x * dt;
				if (transform.position.x >= currNode->GetPosition().x)
				{
					transform.position.x = currNode->GetPosition().x;
					moveState = 3;
				}
			}
			if (currentDirection.left == 1)
			{
				transform.position.x -= MARIO_MAP_VELOCITY.x * dt;
				if (transform.position.x <= currNode->GetPosition().x)
				{
					transform.position.x = currNode->GetPosition().x;
					moveState = 3;
				}
			}
			if (currentDirection.top == 1)
			{
				transform.position.y -= MARIO_MAP_VELOCITY.y * dt;
				if (transform.position.y <= currNode->GetPosition().y)
				{
					transform.position.y = currNode->GetPosition().y;
					moveState = 3;
				}
			}
			if (currentDirection.bottom == 1)
			{
				transform.position.y += MARIO_MAP_VELOCITY.y * dt;
				if (transform.position.y >= currNode->GetPosition().y)
				{
					transform.position.y = currNode->GetPosition().y;
					moveState = 3;
				}
			}
			break;
		}
		case 3:
		{
			moveState = 0;
			currentDirection = { 0, 0, 0, 0 };
			break;
		}
	}
	
}

void CMarioMap::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}

void CMarioMap::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{
	
}

void CMarioMap::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Portal && canEnterScene == true)
	{
		auto portal = static_cast<CPortal*>(otherCollisionBox->GetGameObjectAttach());
		auto sceneID = portal->GetSceneID();
		if (sceneID.compare("") != 0)
		{
			this->sceneID = sceneID;
		}
	}
	if (otherCollisionBox->GetGameObjectAttach()->GetNodeTag() != NodeTag::None && otherCollisionBox->GetGameObjectAttach() != currentNode)
	{
		currentNode = otherCollisionBox->GetGameObjectAttach();
		moveState = 2;
	}
}

bool CMarioMap::SwitchScene()
{
	if (sceneID.compare("world-1-1") == 0)
	{
		CScene1* scene1 = new CScene1();
		CSceneManager::GetInstance()->SwitchScene(scene1);
		return true;
	}
	return false;
}

void CMarioMap::OnKeyDown(int KeyCode)
{
	if (KeyCode == DIK_X)
		canEnterScene = true;
}
