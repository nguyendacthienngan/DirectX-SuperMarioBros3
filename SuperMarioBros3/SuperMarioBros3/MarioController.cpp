#include "MarioController.h"
#include "Game.h"
#include "Mario.h"
#include "MarioControllerConst.h"

#include "SmallMario.h"
#include "SuperMario.h"
#include "RaccoonMario.h"
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
	//SwitchToState(FIRE_MARIO_STATE);
	SwitchToState(RACOON_MARIO_STATE);
	//SwitchToState(SUPER_MARIO_STATE);
	//SwitchToState(SMALL_MARIO_STATE);
}
void CMarioController::Init()
{
	SetTag(GameObjectTags::PlayerController);
	//SetTag(GameObjectTags::Player);
	isEnabled = true;
	CMario* marioStateObject;
	
	// SMALL MARIO
	/*marioStateObject = new CSmallMario();
	listMarioStates.insert(make_pair(SMALL_MARIO_STATE, marioStateObject));
	listStateObjects.insert(make_pair(SMALL_MARIO_STATE, marioStateObject));
	marioStateObject->Enable(false);*/
	
	// SUPER MARIO
	marioStateObject = new CSuperMario();
	listMarioStates.insert(make_pair(SUPER_MARIO_STATE, marioStateObject));
	listStateObjects.insert(make_pair(SUPER_MARIO_STATE, marioStateObject));
	marioStateObject->Enable(false);

	// RACOON MARIO
	marioStateObject = new CRaccoonMario();
	listMarioStates.insert(make_pair(RACOON_MARIO_STATE, marioStateObject));
	listStateObjects.insert(make_pair(RACOON_MARIO_STATE, marioStateObject));
	marioStateObject->Enable(false);

	// FIRE MARIO
	marioStateObject = new CFireMario();
	listMarioStates.insert(make_pair(FIRE_MARIO_STATE, marioStateObject));
	listStateObjects.insert(make_pair(FIRE_MARIO_STATE, marioStateObject));
	marioStateObject->Enable(false);

	collisionBoxs->clear();
	//collisionBoxs = NULL;

	physiscBody->SetDynamic(false);

}

void CMarioController::Process()
{
	CStateMachine::Process();
}

void CMarioController::AddStateObjectsToScene(LPScene scene)
{
	for (auto obj : listStateObjects)
	{
		scene->AddObject(obj.second);
		obj.second->AddObjectToScene(scene);
	}
}

void CMarioController::SwitchToState(std::string state)
{
	// Chưa check điều kiện input

	// Đổi trạng thái (STATE)
		SwitchState(listMarioStates.at(state)); 

	// Nếu object đó đã được khởi tạo trc đó thì mình lấy lại vị trí trc đó đã lưu
	if (currentStateObject != NULL)
	{
		auto controllerPhyBody = currentStateObject->GetPhysiscBody();
		// Của current state object (SuperMario, SmallMario,..)
		auto currentObj = listStateObjects.at(state);
		auto currentPhyBody = currentObj->GetPhysiscBody();

		currentPhyBody->SetVelocity(controllerPhyBody->GetVelocity());
		currentPhyBody->SetGravity(controllerPhyBody->GetGravity());
		currentPhyBody->SetAcceleration(controllerPhyBody->GetAcceleration());

		DebugOut(L"When switch state, currentStateObj %f, %f \n", currentStateObject->GetPosition().x, currentStateObject->GetPosition().y);
		listStateObjects.at(state)->SetPosition(currentStateObject->GetPosition());

		D3DXVECTOR2 transform = D3DXVECTOR2(0.0f, 0.0f);
		transform.y = SUPER_MARIO_BBOX.y - SMALL_MARIO_BBOX.y; // Tính lại

		// hoặc boxsize coi của nhỏ hay lớn
		if (listStateObjects.at(state)->GetTag() == GameObjectTags::SmallPlayer)
		{
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

#pragma region PassParamater For DamageProcess
		auto stateMarioToBeChanged = static_cast<CMario*>(listStateObjects.at(state));
		auto currentMario = static_cast<CMario*>(currentStateObject);
		stateMarioToBeChanged->SetDamageFlag(currentMario->GetDamageFlag());
		stateMarioToBeChanged->SetChangeSmokeEffectFlag(currentMario->GetChangeSmokeEffectFlag());
		stateMarioToBeChanged->SetChangeLevelFlag(currentMario->GetChangeLevelFlag());

		stateMarioToBeChanged->SetCountChangeAlpha(currentMario->GetCountChangeAlpha());
		stateMarioToBeChanged->SetCountSmokeEffectActivate(currentMario->GetCountSmokeEffectActivate());

		stateMarioToBeChanged->SetTimeStartDamaged(currentMario->GetTimeStartDamaged());
		stateMarioToBeChanged->SetTimeStartSmokeEffect(currentMario->GetTimeStartSmokeEffect());
		stateMarioToBeChanged->SetTimeStartChangeLevel(currentMario->GetTimeStartChangeLevel());
		stateMarioToBeChanged->SetTimeStartChangeAlpha(currentMario->GetTimeStartChangeAlpha());
		stateMarioToBeChanged->SetPowerUp(currentMario->IsPowerUp());
#pragma endregion

		auto curMario = static_cast<CMario*>(currentStateObject);
		if (curMario->GettMarioStateTag() == MarioStates::RacoonMario)
		{
			auto curRacconMario = static_cast<CRaccoonMario*>(curMario);
			curRacconMario->ResetValueAttack();
			
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
	if (KeyCode == DIK_1)
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
	/*else if (KeyCode == DIK_4)
	{
		SwitchToState(SMALL_MARIO_STATE);
	}*/
}

void CMarioController::Update(DWORD dt, CCamera* cam)
{
	Process();
	if (currentStateObject == NULL)
		return;
	this->transform.position = currentStateObject->GetPosition();
	this->physiscBody->SetVelocity(currentStateObject->GetPhysiscBody()->GetVelocity());
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
