#include "MarioController.h"
#include "Game.h"
#include "Mario.h"
#include "MarioControllerConst.h"

#include "SmallMario.h"
#include "SuperMario.h"
#include "RacoonMario.h"
#include "FireMario.h"
#include "GameObjectTags.h"
#include "MarioConst.h"
#include "SceneManager.h"
#include <unordered_map>
#include "Ultis.h"

using namespace std;
CMarioController::CMarioController()
{
	Init();
	currentStateObject = NULL;
	SwitchToState(RACOON_MARIO_STATE);
	//SwitchToState(SUPER_MARIO_STATE);
	//SwitchToState(SMALL_MARIO_STATE);
}
void CMarioController::Init()
{
	SetTag(GameObjectTags::Player);

	CMario* marioStateObject;
	
	//// SMALL MARIO
	//marioStateObject = new CSmallMario();
	//listMarioStates.insert(make_pair(SMALL_MARIO_STATE, marioStateObject));
	//listStateObjects.insert(make_pair(SMALL_MARIO_STATE, marioStateObject));
	//marioStateObject->Enable(false);
	//
	//// SUPER MARIO
	//marioStateObject = new CSuperMario();
	//listMarioStates.insert(make_pair(SUPER_MARIO_STATE, marioStateObject));
	//listStateObjects.insert(make_pair(SUPER_MARIO_STATE, marioStateObject));
	//marioStateObject->Enable(false);

	// RACOON MARIO
	marioStateObject = new CRacoonMario();
	listMarioStates.insert(make_pair(RACOON_MARIO_STATE, marioStateObject));
	listStateObjects.insert(make_pair(RACOON_MARIO_STATE, marioStateObject));
	marioStateObject->Enable(false);

	//// FIRE MARIO
	//marioStateObject = new CFireMario();
	//listMarioStates.insert(make_pair(FIRE_MARIO_STATE, marioStateObject));
	//listStateObjects.insert(make_pair(FIRE_MARIO_STATE, marioStateObject));
	//marioStateObject->Enable(false);
}

void CMarioController::Process()
{
	CStateMachine::Process();
	if (currentStateObject == NULL)
		return;


	// Lấy ra position của current object, MarioController đi theo
	SetPosition(currentStateObject->GetPosition());

	for (auto stateObj : listStateObjects)
	{
		auto obj = (stateObj).second; // Lấy ra obj của stateobj

		if (obj == currentStateObject)
			continue;
		D3DXVECTOR2 transform = D3DXVECTOR2(0.0f, 0.0f);
	}
	
}

void CMarioController::AddStateObjectsToScene(LPScene scene)
{
	for (auto obj : listStateObjects)
		scene->AddObject(obj.second);
}

void CMarioController::SwitchToState(std::string state)
{
	// Đổi trạng thái (STATE)
		SwitchState(listMarioStates.at(state)); 

	// Nếu object đó đã được khởi tạo trc đó thì mình lấy lại vị trí trc đó đã lưu
	if (currentStateObject != NULL)
	{
		// Current state object sẽ đi theo Mario Controller

		auto controllerPhyBody = currentStateObject->GetPhysiscBody();

		// Của current state object (SuperMario, SmallMario,..)
		auto currentObj = listStateObjects.at(state);
		auto currentPhyBody = currentObj->GetPhysiscBody();

		currentPhyBody->SetVelocity(controllerPhyBody->GetVelocity());
		currentPhyBody->SetGravity(controllerPhyBody->GetGravity());
		currentPhyBody->SetAcceleration(controllerPhyBody->GetAcceleration());

		listStateObjects.at(state)->SetPosition(currentStateObject->GetPosition());
		D3DXVECTOR2 transform = D3DXVECTOR2(0.0f, 0.0f);
		transform.y = SUPER_MARIO_BBOX.y - SMALL_MARIO_BBOX.y; // Tính lại

		// hoặc boxsize coi của nhỏ hay lớn
		if (listStateObjects.at(state)->GetTag() == GameObjectTags::SmallMario)
		{
			//listStateObjects.at(state)->SetPosition(listStateObjects.at(state)->GetPosition() + transform);
			listStateObjects.at(state)->SetPosition(listStateObjects.at(state)->GetPosition() + transform);
			listStateObjects.at(state)->GetCollisionBox()->at(0)->SetPosition(D3DXVECTOR2(0.0f, 0.0f));
			listStateObjects.at(state)->SetRelativePositionOnScreen(listStateObjects.at(state)->GetCollisionBox()->at(0)->GetPosition());


		}
		else
		{
			listStateObjects.at(state)->SetPosition(listStateObjects.at(state)->GetPosition() - transform);
			 listStateObjects.at(state)->GetCollisionBox()->at(0)->SetPosition(transform);
			listStateObjects.at(state)->SetRelativePositionOnScreen(listStateObjects.at(state)->GetCollisionBox()->at(0)->GetPosition());

		}

	}

	// Gán object (OBJECT)
	if (listStateObjects.size() != 0)
	{
		currentStateObject = listStateObjects.at(state);
		auto scene = CSceneManager::GetInstance()->GetActiveScene();
		if (scene != NULL)
		{
			auto camera = scene->GetCamera();
			camera->SetGameObject(currentStateObject);
		}
	}
		
}

void CMarioController::OnKeyDown(int KeyCode)
{
	/*if (KeyCode == DIK_1)
	{
		SwitchToState(SUPER_MARIO_STATE);
	}
	else if (KeyCode == DIK_2)
	{
		SwitchToState(RACOON_MARIO_STATE);
	}
	else if (KeyCode == DIK_3)
	{
		SwitchToState(FIRE_MARIO_STATE);
	}
	else if (KeyCode == DIK_4)
	{
		SwitchToState(SMALL_MARIO_STATE);

	}*/
}

void CMarioController::SetCurrentStateObject(LPGameObject gO)
{
	this->currentStateObject = gO;
}

LPGameObject CMarioController::GetCurrentStateObject()
{
	return currentStateObject;
}

CMarioController::~CMarioController()
{
	
}
