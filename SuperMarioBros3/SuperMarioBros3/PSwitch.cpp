#include "PSwitch.h"
#include "Game.h"
#include "PSwitchConst.h"
#include "AnimationManager.h"
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
	if (listBricks.size() <= 0 || listCoins.size() <= 0)
		return;
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
			for (auto coin : listCoins)
			{
				coin->Enable(false);
			}
			for (auto brick : listBricks)
			{
				brick->Enable(true);
			}
			timer = 0;
			break;
		}
	}
}

void CPSwitch::Active()
{
	if (changeState != 0 || listBricks.size() <= 0)
		return;
	this->physiscBody->SetDynamic(false);
	this->collisionBoxs->at(0)->SetEnable(false);
	timer = 0;
	for (auto brick : listBricks)
	{
		brick->Enable(false);
	}
	for (auto coin : listCoins)
	{
		coin->Enable(true);
	}
	changeState = 1;
}

bool CPSwitch::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	return false;
}

void CPSwitch::SetListBricks(std::vector<CBrick*> listBricks)
{
	this->listBricks = listBricks;
}

void CPSwitch::SetListCoins(std::vector<CCoin*> listCoins)
{
	this->listCoins = listCoins;
}
