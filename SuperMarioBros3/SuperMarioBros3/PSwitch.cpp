#include "PSwitch.h"
#include "Game.h"
#include "PSwitchConst.h"
#include "AnimationManager.h"
#include "SceneManager.h"
CPSwitch::CPSwitch()
{
	LoadAnimation();
	isEnabled = true;
	tag = GameObjectTags::SwitchBlock;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(PSWITCH_SIZE_BOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Swtich-Block");
	collisionBox->SetPosition(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(0.0f);

	changeState = 0;
	timer = 0;
}

void CPSwitch::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("ACTIVE", animationManager->Clone("ani-switch-block-blue-active"));
	AddAnimation("INACTIVE", animationManager->Clone("ani-switch-block-blue-inactive"));
	SetState("ACTIVE");
}

void CPSwitch::Render(CCamera* cam, int alpha)
{
	if (changeState != 0)
	{
		SetState("INACTIVE");
		collisionBoxs->at(0)->SetSizeBox(PSWITCH_SIZE_BOX_INACTIVE);
		collisionBoxs->at(0)->SetDistance(D3DXVECTOR2(0.0f, PSWITCH_SIZE_BOX.y*0.5f - PSWITCH_SIZE_BOX_INACTIVE.y * 0.5f));
		SetRelativePositionOnScreen(D3DXVECTOR2(0.0f, PSWITCH_SIZE_BOX.y * 0.5f - PSWITCH_SIZE_BOX_INACTIVE.y * 0.5f));
	}
	CGameObject::Render(cam, alpha);
}

void CPSwitch::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	switch (changeState)
	{
		case 1:
		{
			// Set Time
			timer += CGame::GetInstance()->GetDeltaTime();
			if (timer >= PSWITCH_TIME_TO_ACTIVE)
				changeState = 2;
			break;
		}
		case 2:
		{
			auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
			if (activeScene != NULL)
			{
				activeScene->SwitchBlockStateOnToOff(true);
				auto coins = activeScene->GetCoins();
				auto poolBricks = activeScene->GetPoolBricks();

				for (auto coin : coins)
				{
					if (coin == NULL) continue;
					if (coin->IsEnabled() == false) continue;
					coin->Enable(false);
					coin->GetPhysiscBody()->SetDynamic(false);
					coin->GetCollisionBox()->at(0)->SetEnable(false);

					auto brick = poolBricks->Init();
					brick->Enable(true);
					brick->SetPosition(coin->GetPosition());
					brick->GetPhysiscBody()->SetDynamic(true);
					brick->GetCollisionBox()->at(0)->SetEnable(true);
				}
				
			}
			timer = 0;
			break;
		}
	}
}

void CPSwitch::Active()
{
	if (changeState != 0)
		return;
	timer = 0;
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene != NULL)
	{
		activeScene->SwitchBlockStateOnToOff(false);
		auto bricks = activeScene->GetBricks();
		auto coins = activeScene->GetCoins();
		auto poolCoins = activeScene->GetPoolCoins();
		for (auto brick : bricks)
		{
			brick->Enable(false);
			brick->GetPhysiscBody()->SetDynamic(false);
			brick->GetCollisionBox()->at(0)->SetEnable(false);
			auto coin = poolCoins->Init();
			if (coin != NULL)
			{
				coin->SetPosition(brick->GetPosition());
				activeScene->AddCoin(coin);
			}
		}
	}
	changeState = 1;
}

bool CPSwitch::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	return false;
}

int CPSwitch::GetChangeState()
{
	return changeState;
}
