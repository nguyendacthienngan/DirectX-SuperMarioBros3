#include "MarioScript.h"
#include "SceneManager.h"
#include "MarioControllerConst.h"
#include "MarioConst.h"
CMarioScript::CMarioScript()
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();

	marioController = new CMarioAuto();
	marioController->SetPosition(D3DXVECTOR2(743, 450));
	marioController->AddStateObjectsToScene(activeScene);
	marioController->SwitchToState(SUPER_MARIO_STATE);
	marioController->GetCurrentStateObject()->SetPosition(D3DXVECTOR2(743, 450));
	marioController->GetCurrentStateObject()->GetPhysiscBody()->SetNormal(D3DXVECTOR2(-1, 1));
	activeScene->AddObject(marioController);

	luigi = new CLuigiAuto();
	luigi->SetPosition(D3DXVECTOR2(24, 450));

	activeScene->AddObject(luigi);

	transform.position = D3DXVECTOR2(350, 300);
	marioStage = -1;
	luigiStage = -1;
	isEnabled = true;
}

void CMarioScript::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	LuigiScript(dt, cam, uiCam);
	MarioScript(dt, cam, uiCam);
}

void CMarioScript::Render(CCamera* cam, int alpha)
{
}

void CMarioScript::SetStartAction()
{
	marioStage = 0;
	luigiStage = 0;
}

void CMarioScript::MarioScript(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto currentMario = static_cast<CMario*> (marioController->GetCurrentStateObject());
	/*currentMario->SetAutoGo(true);
	currentMario->SetIsInIntro(true);*/
	switch (marioStage)
	{
		case 0:
		{
			auto phyBody = currentMario->GetPhysiscBody();
			auto vel = phyBody->GetVelocity();
			auto pos = currentMario->GetPosition();

			if (pos.x >= 400)
			{
				vel.x = -MARIO_WALKING_SPEED * 1.5f;
				pos.x += vel.x * dt;
				currentMario->SetPosition(pos);

				MarioStateSet phyState;
				phyState.move = MoveOnGroundStates::Walk;
				phyState.jump = JumpOnAirStates::Stand;
				currentMario->SetPhysicsState(phyState);
			}
			else
			{
				MarioStateSet phyState;
				phyState.move = MoveOnGroundStates::Crouch;
				phyState.jump = JumpOnAirStates::Stand;
				auto luigiPos = luigi->GetPosition();
				if (luigiPos.x >= pos.x )
				{
					currentMario->SetPhysicsState(phyState);
					marioStage++;
				}
				
			}
			break;
		}
		case 1:
		{
			MarioStateSet phyState;
			phyState.move = MoveOnGroundStates::Idle;
			phyState.jump = JumpOnAirStates::Stand;
			auto luigiPos = luigi->GetPosition();
			if (luigiPos.x > 600)
				currentMario->SetPhysicsState(phyState);
			marioStage++;

			break;
		}
		case 2:
		{
			auto phyBody = currentMario->GetPhysiscBody();
			auto vel = phyBody->GetVelocity();
			auto pos = currentMario->GetPosition();

			if (pos.x >= 24)
			{
				vel.x = -MARIO_WALKING_SPEED * 1.5f;
				pos.x += vel.x * dt;
				currentMario->SetPosition(pos);

				MarioStateSet phyState;
				phyState.move = MoveOnGroundStates::Walk;
				phyState.jump = JumpOnAirStates::Stand;
				currentMario->SetPhysicsState(phyState);


			}
			else
				currentMario->Enable(false);
			break;
		}
	}
}

void CMarioScript::LuigiScript(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto currentLuigi = static_cast<CLuigiAuto*> (luigi);

	switch (luigiStage)
	{
		case 0:
		{
			auto phyBody = currentLuigi->GetPhysiscBody();
			auto vel = phyBody->GetVelocity();
			auto pos = currentLuigi->GetPosition();
			if (pos.x <= 190)
			{
				// Đi bộ tới mario đỏ
				vel.x = MARIO_WALKING_SPEED;
				pos.x += vel.x * dt;
				currentLuigi->SetPosition(pos);

				MarioStateSet phyState;
				phyState.move = MoveOnGroundStates::Walk;
				phyState.jump = JumpOnAirStates::Stand;
				currentLuigi->SetPhysicsState(phyState);

			}
			else
			{
				auto currentPhyState = currentLuigi->GetPhysicsState();

				// Nhảy lên đầu mario 
				if (currentPhyState.jump == JumpOnAirStates::Stand)
				{
					pos.x += MARIO_WALKING_SPEED * dt;
					vel.y = -MARIO_PUSH_FORCE*2;
					pos.y += vel.y * dt;

					currentPhyState.jump = JumpOnAirStates::Jump;
				}
				else if (currentPhyState.jump == JumpOnAirStates::Jump)
				{
					pos.x += MARIO_WALKING_SPEED * dt;
					vel.y = -MARIO_PUSH_FORCE * 2;
					pos.y += vel.y * dt;

				}
				if (pos.y > 300)
				{
					currentPhyState.jump = JumpOnAirStates::Fall;
				}
				if (pos.x > 800)
					luigiStage++;
				currentLuigi->SetPosition(pos);
				currentLuigi->SetPhysicsState(currentPhyState);
			}

			break;
		}
	}
}

D3DXVECTOR2 CMarioScript::GetLuigiPosition()
{
	return luigi->GetPosition();
}
