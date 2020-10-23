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
#include <unordered_map>

using namespace std;
CMarioController::CMarioController()
{
	Init();
	currentStateObject = NULL;
	SwitchToState(SUPER_MARIO_STATE);
	currentStateObject->Enable(true);
}
void CMarioController::Init()
{
	SetTag(GameObjectTags::Player);

	CMario* marioStateObject;
	
	// SMALL MARIO
	marioStateObject = new CSmallMario();
	listMarioStates.insert(make_pair(SMALL_MARIO_STATE, marioStateObject));
	listStateObjects.insert(make_pair(SMALL_MARIO_STATE, marioStateObject));
	marioStateObject->Enable(false);
	
	// SUPER MARIO
	marioStateObject = new CSuperMario();
	listMarioStates.insert(make_pair(SUPER_MARIO_STATE, marioStateObject));
	listStateObjects.insert(make_pair(SUPER_MARIO_STATE, marioStateObject));
	marioStateObject->Enable(false);

	// RACOON MARIO
	/*marioStateObject = new CRacoonMario();
	listMarioStates.insert(make_pair(RACOON_MARIO_STATE, marioStateObject));
	listStateObjects.insert(make_pair(RACOON_MARIO_STATE, marioStateObject));
	marioStateObject->Enable(false);*/

	// FIRE MARIO
	marioStateObject = new CFireMario();
	listMarioStates.insert(make_pair(FIRE_MARIO_STATE, marioStateObject));
	listStateObjects.insert(make_pair(FIRE_MARIO_STATE, marioStateObject));
	marioStateObject->Enable(false);
}

void CMarioController::Update()
{
	CStateMachine::Update();
	if (currentStateObject == NULL)
		return;

	// Lấy ra position của current object, MarioController đi theo
	SetPosition(currentStateObject->GetPosition());

	for (auto stateObj : listStateObjects)
	{
		auto obj = (stateObj).second; // Lấy ra obj của stateobj

		if (obj == currentStateObject) // ?
			continue;
		D3DXVECTOR2 transform = D3DXVECTOR2(0.0f, 0.0f);

		// Set lại box size: Mario chỉ có 2 box size là nhỏ với lớn thôi, nên chỉ cần xét small mario và những anh bạn còn lại

		if (obj->GetTag() == GameObjectTags::SmallMario || currentStateObject->GetTag() == GameObjectTags::SmallMario)
			transform = SUPER_MARIO_BBOX - SMALL_MARIO_BBOX;
		if (obj->GetTag() == GameObjectTags::SmallMario)
			obj->SetPosition(currentStateObject->GetPosition() + transform);
		else
			obj->SetPosition(currentStateObject->GetPosition() - transform);
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
	//if (state.compare("") != 0)
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
	}

	// Gán object (OBJECT)
	if (listStateObjects.size() != 0)
		currentStateObject = listStateObjects.at(state);
}

void CMarioController::OnKeyDown(int KeyCode)
{
	if (KeyCode == DIK_1)
	{
		SwitchToState(SMALL_MARIO_STATE);
	}
	else if (KeyCode == DIK_2)
	{
		SwitchToState(SUPER_MARIO_STATE);
	}
	else if (KeyCode == DIK_3)
	{
		SwitchToState(RACOON_MARIO_STATE);
	}
	else if (KeyCode == DIK_4)
	{
		SwitchToState(FIRE_MARIO_STATE);
	}
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
