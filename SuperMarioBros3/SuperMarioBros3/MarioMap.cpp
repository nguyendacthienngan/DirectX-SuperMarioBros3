#include "MarioMap.h"
#include "AnimationManager.h"
#include "MarioMapConst.h"
#include "KeyboardManager.h"
#include "Portal.h"
#include "SceneManager.h"
#include "Scene1.h"
CMarioMap::CMarioMap()
{
	LoadAnimation();
	//SetTag(GameObjectTags::SmallPlayer);
	SetState(SMALL_MARIO_MAP_STATE);
	isEnabled = true;
	physiscBody->SetDynamic(true);
	//physiscBody->SetGravity(0.0f);
	direction = { 1, 1, 1, 1 };

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(D3DXVECTOR2(14*3,16*3));
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Small-Mario-Map"); // SmallMario
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	//SetRelativePositionOnScreen(D3DXVECTOR2(0.0f, SUPER_MARIO_BBOX.y / 2 - SMALL_MARIO_BBOX.y / 2));
	this->collisionBoxs->push_back(collisionBox);

	sceneID = "";
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
	auto keyboard = CKeyboardManager::GetInstance();
	float vel = 1.0f;
	if (keyboard->GetKeyStateDown(DIK_RIGHT) && direction.right == 1)
	{
		transform.position.x += vel * dt;
	}
	if (keyboard->GetKeyStateDown(DIK_LEFT) && direction.left == 1)
	{
		transform.position.x -= vel * dt;
	}
	if (keyboard->GetKeyStateDown(DIK_UP) && direction.top == 1)
	{
		transform.position.y -= vel * dt;
	}
	if (keyboard->GetKeyStateDown(DIK_DOWN) && direction.bottom == 1)
	{
		transform.position.y += vel * dt;
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
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Portal)
	{
		auto portal = static_cast<CPortal*>(otherCollisionBox->GetGameObjectAttach());
		auto sceneID = portal->GetSceneID();
		if (sceneID.compare("") != 0)
		{
			this->sceneID = sceneID;
			
		}
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
