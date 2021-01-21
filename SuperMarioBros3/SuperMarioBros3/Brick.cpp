#include "Brick.h"
#include "AnimationManager.h"
#include "BrickConst.h"
#include "SceneManager.h"
#include "BrickEffect.h"

#include "CoinEffect.h"
#include "SceneManager.h"
#include "Ultis.h"
#include "Game.h"
#include "LeafEffect.h"
#include "MushroomEffect.h"
#include "Mario.h"
#include "MarioController.h"
#include "FireFlower.h"
#include "BlockConst.h"
#include "PSwitch.h"

#include "QuestionBlockConst.h"

CBrick::CBrick()
{
	LoadAnimation();
	Init();
	SetState(BRICK_STATE_IDLE);
	isEnabled = true;
	startBounceTime = 0;
	bounceState = 0;
	bounceDelta = 0.0f;
	for (int i = 0; i < 4; i++)
	{
		CBrickEffect* brickFX = new CBrickEffect();
		brickFX->LinkToPool(&brickPool);
		brickPool.Add(brickFX);
	}
	itemInfo.tag = ItemTag::None;
	itemInfo.quantity = 0;
}

void CBrick::SetTarget(CGameObject* target)
{
	this->target = target;
}

void CBrick::SetItemInfo(ItemInfo info)
{
	this->itemInfo = info;
}

ItemInfo CBrick::GetItemInfo()
{
	return itemInfo;
}

void CBrick::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation(BRICK_STATE_IDLE, animations->Get("ani-brick"));
	AddAnimation(BRICK_STATE_EMPTY, animations->Get("ani-empty-block"));
}

void CBrick::Init()
{
	tag = GameObjectTags::Brick;
	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(BRICK_BBOX);
	box->SetGameObjectAttach(this);
	this->collisionBoxs->push_back(box);
	this->physiscBody->SetDynamic(true);
}

void CBrick::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	if (bounceState == 2)
	{
		bounceDelta += BOUNCE_VEL * dt;

		if (GetTickCount64() - startBounceTime > BOUNCE_TIME && startBounceTime != 0)
		{
			bounceDelta = 0.0f;
			startBounceTime = 0;
			bounceState = 0;
		}
	}

	if (bounceState == 1)
	{
		bounceDelta += -BOUNCE_VEL * dt;

		if (GetTickCount64() - startBounceTime > BOUNCE_TIME && startBounceTime != 0)
		{
			bounceState = 2;
			startBounceTime = GetTickCount64();
		}
	}
}

void CBrick::Render(CCamera* cam, int alpha)
{
	relativePositionOnScreen.y = bounceDelta;
	CGameObject::Render(cam, alpha);
}

void CBrick::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::RaccoonTail)
	{
		Bounce();
		Debris();
	}
}

void CBrick::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
}

bool CBrick::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	return false;
}

void CBrick::SetType(int type)
{
	this->type = type;
}

int CBrick::GetType()
{
	return type;
}

void CBrick::Debris()
{
	if (itemInfo.tag != ItemTag::None)
		return;
	const float velx[4] = { +0.1875f, +0.25f, -0.25f, -0.1875f };
	const float vely[4] = { -0.375f, -0.75f, -0.75f, -0.375f };

	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();

	for (int i = 0; i < 4; i++)
	{
		auto brickFX = brickPool.Init();
		if (brickFX != NULL)
		{
			auto pos = brickFX->GetPosition();
			brickFX->GetPhysiscBody()->SetVelocity(D3DXVECTOR2(velx[i], vely[i]));
			brickFX->Enable(true);
			brickFX->SetPosition(this->GetPosition());

			auto grid = activeScene->GetGrid();
			if (grid != NULL && activeScene->IsSpacePartitioning() == true)
				grid->Move(pos, brickFX);
		}
	}
	isEnabled = false;
	activeScene->RemoveBrick(this);
	activeScene->RemoveObject(this);
	activeScene->AddDestroyObject(this);
}

void CBrick::Bounce()
{
	if (bounceState != 0)
		return;
	startBounceTime = GetTickCount64();
	bounceState = 1;

	if (itemInfo.quantity > 0)
	{
		switch (itemInfo.tag)
		{
		case ItemTag::Coin:
		{
			CCoinEffect* coinObtainedFX = new CCoinEffect();
			coinObtainedFX->SetStartPosition(transform.position);
			auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
			activeScene->AddObject(coinObtainedFX);
			activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), coinObtainedFX);
			OnScoreEffect();
			break;
		}
		case ItemTag::PowerUp:
		{
			// có thể mario va chạm, hoặc là mai rùa va chạm theo chiều ngang
			// nếu là mai rùa thì làm sao mình có thể xét theo tag của mario?
			// question block phải giữ Mario Controller chăng?
			if (target == NULL)
				break;
			auto marioController = static_cast<CMarioController*>(target);
			auto currentMario = marioController->GetCurrentStateObject();
			if (currentMario != NULL)
			{
				auto mario = static_cast<CMario*>(currentMario);
				switch (mario->GettMarioStateTag())
				{
				case MarioStates::SmallMario:
				{
					CMushroomEffect* mushroomObtainedFX = new CMushroomEffect();
					mushroomObtainedFX->SetStartPosition(transform.position);
					mushroomObtainedFX->StartEffect(mario->GetPhysiscBody()->GetNormal().x);
					auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
					activeScene->AddObject(mushroomObtainedFX);
					activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), mushroomObtainedFX);
					break;
				}
				case MarioStates::SuperMario:
				{
					srand(time(NULL));
					int res = rand() % (2) + 0;
					if (res == 0)
					{
						CLeafEffect* leafObtainedFX = new CLeafEffect();
						leafObtainedFX->SetStartPosition(transform.position);
						leafObtainedFX->StartEffect();
						auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
						activeScene->AddObject(leafObtainedFX);
						activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), leafObtainedFX);

					}
					if (res == 1)
					{
						CFireFlower* fireFlower = new CFireFlower();
						D3DXVECTOR2 firePos = transform.position;
						firePos.y -= BLOCK_BBOX.y;
						fireFlower->SetStartPosition(firePos);
						auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
						activeScene->AddObject(fireFlower);
						activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), fireFlower);
					}
					break;
				}
				case MarioStates::FireMario:
				{
					CLeafEffect* leafObtainedFX = new CLeafEffect();
					leafObtainedFX->SetStartPosition(transform.position);
					leafObtainedFX->StartEffect();
					auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
					activeScene->AddObject(leafObtainedFX);
					activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), leafObtainedFX);
					break;
				}
				case MarioStates::RacoonMario:
				{
					CFireFlower* fireFlower = new CFireFlower();
					D3DXVECTOR2 firePos = transform.position;
					firePos.y -= BLOCK_BBOX.y;
					fireFlower->SetStartPosition(firePos);
					auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
					activeScene->AddObject(fireFlower);
					activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), fireFlower);
					break;
				}
				}
			}

			break;
		}
		case ItemTag::PSwitch:
		{
			CPSwitch* pSwitch = new CPSwitch();
			auto pos = transform.position;
			pos.y -= BRICK_BBOX.y;
			pSwitch->SetPosition(pos);
			auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
			activeScene->AddObject(pSwitch);
			activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), pSwitch);
			break;
		}
		}
		itemInfo.quantity--;
		if (itemInfo.quantity <= 0)
		{
			if (itemInfo.quantity < 0)
				itemInfo.quantity = 0;
			if (currentState != BRICK_STATE_EMPTY)
				SetState(BRICK_STATE_EMPTY);
		}
	}
}

CObjectPool CBrick::GetObjectPool()
{
	return brickPool;
}

