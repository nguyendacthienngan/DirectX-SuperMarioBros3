#include "BoomerangBrother.h"
#include "AnimationManager.h"
#include "BoomerangBrotherConst.h"
#include "Boomerang.h"
#include "SceneManager.h"
#include "Game.h"

CBoomerangBrother::CBoomerangBrother()
{
	enemyTag = EnemyTag::BoomerangBrother;
	LoadAnimation();
	SetState(BOOMERANG_STATE_MOVE);
	isEnabled = true;
	for (int i = 0; i < 2; i++)
	{
		CBoomerang* boomerang = new CBoomerang();
		boomerang->LinkToPool(&boomerangs);
		boomerang->SetBoomerangBrother(this);
		boomerangs.Add(boomerang);
	}
	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(BOOMERANG_BROTHER__BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Boomerang-Brother");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(BOOMERANG_BROTHER_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));

	moveState = 1;
	canAttack = false;
	idleTimer = 0;
	onHoldObject = NULL;
	isHoldBoomerang = false;
}

void CBoomerangBrother::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(BOOMERANG_STATE_MOVE, animationManager->Clone("ani-boomerang-brother-move"));
	AddAnimation(BOOMERANG_STATE_ATTACK, animationManager->Clone("ani-boomerang-brother-attack"));
	AddAnimation(BOOMERANG_STATE_IDLE, animationManager->Clone("ani-boomerang-brother-idle"));
}

void CBoomerangBrother::Render(CCamera* cam, int alpha)
{
	auto normal = physiscBody->GetNormal();
	SetScale(D3DXVECTOR2(-normal.x, 1.0f));

	if (isHoldBoomerang == true)
		SetState(BOOMERANG_STATE_ATTACK);
	else if (idleTimer != 0)
		SetState(BOOMERANG_STATE_IDLE);
	else
		SetState(BOOMERANG_STATE_MOVE);
	CGameObject::Render(cam, alpha);
}

void CBoomerangBrother::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto distanceBetweenTargetAndBoomerang = target->GetPosition().x - this->transform.position.x;
	if (abs(distanceBetweenTargetAndBoomerang) <= DISTANCE_CAN_THROW_TWICE)
		canThrowSecondBoomerang = true;

	auto normal = physiscBody->GetNormal();
	auto targetPos = D3DXVECTOR2(0, 0);
	if (target != NULL)
	{
		normal.x = (target->GetPosition().x < this->transform.position.x) ? -1 : 1;
		targetPos = target->GetPosition();
	}
	physiscBody->SetNormal(normal);
	switch (moveState)
	{
		case 1:
		{
			// Tiến về mario. Sau khi tiến xong thì đó là cột mốc X
			
			OnMovingForward(normal);

			break;
		}
		case 2:
		{
			// Lùi
			OnMovingBackwards(normal);

			break;
		}
		case 3:
		{
			// Tiến, quay về vị trí ban đầu (cột mốc X)
			OnMovingForward(normal);
			break;
		}
		case 4:
		{
			// Lùi
			OnMovingBackwards(normal);
			break;

		}
	}

	if (canThrowSecondBoomerang == false)
	{
		if (isHoldBoomerang == false)
		{
			if (moveState == 2 || moveState == 4)
			{
				OnHoldBoomerang(normal);
				isHoldBoomerang = true;
				canAttack = false;
			}
		}
		else
		{
			if (moveState == 1 || moveState == 3)
			{
				isHoldBoomerang = false;
				canAttack = true;
			}
		}
	}
	
	
	if (canAttack == true)
	{
		OnAttack(normal);
	}
	DebugOut(canThrowSecondBoomerang == true ? L"SECOND BOOMERANG \n" : L"ONE BOOMERANG \n");
}

CObjectPool CBoomerangBrother::GetObjectPool()
{
	return boomerangs;
}

void CBoomerangBrother::OnAttack(D3DXVECTOR2 normal)
{
	if (onHoldObject != NULL)
	{
		auto currentBoomerang = static_cast<CBoomerang*>(onHoldObject);
		currentBoomerang->SetAttackState(0);
		onHoldObject = NULL;
		canAttack = false;
	}
}

void CBoomerangBrother::OnMovingForward(D3DXVECTOR2 normal)
{
	if (transform.position.x >= startPosition.x + BOUNDARY)
	{
		if (canThrowSecondBoomerang == true)
		{
			if (isHoldBoomerang == true)
			{
				isHoldBoomerang = false;
				OnAttack(normal);
			}
		}
		idleTimer += CGame::GetInstance()->GetDeltaTime();
		if (idleTimer > IDLE_TIME)
		{
			idleTimer = 0;
			moveState++;
			startPosition.x = transform.position.x;
		}

	}
	else
	{
		float velX = BOOMERANG_BROTHER_VELOCITY * normal.x;
		transform.position.x += velX * CGame::GetInstance()->GetDeltaTime();
		if (transform.position.x - startPosition.x >= BOUNDARY / 2)
		{
			if (canThrowSecondBoomerang == true)
			{
				if (isHoldBoomerang == false)
				{
					OnHoldBoomerang(normal);
					isHoldBoomerang = true;
					canAttack = false;
				}
			}
		}
	}
}

void CBoomerangBrother::OnMovingBackwards(D3DXVECTOR2 normal)
{
	auto distanceBetweenTargetAndBoomerang = target->GetPosition().x - this->transform.position.x;
	if (abs(distanceBetweenTargetAndBoomerang) <= DISTANCE_CAN_THROW_TWICE)
		canThrowSecondBoomerang = true;

	// Xét biên
	if (transform.position.x <= startPosition.x - BOUNDARY)
	{
		if (canThrowSecondBoomerang == true)
		{
			if (isHoldBoomerang == true)
			{
				isHoldBoomerang = false;
				OnAttack(normal);
			}
		}

		idleTimer += CGame::GetInstance()->GetDeltaTime();
		if (idleTimer > IDLE_TIME)
		{
			
			if (moveState == 2)
			{
				if (canThrowSecondBoomerang == true)
					moveState++;
				else
				{
					moveState = 1;
				}
			}
			else if (moveState == 4)
			{
				moveState = 1;
			}

			idleTimer = 0;
			startPosition.x = transform.position.x;

		}
	}
	else
	{
		float velX = -BOOMERANG_BROTHER_VELOCITY * normal.x;
		transform.position.x += velX * CGame::GetInstance()->GetDeltaTime();

		if (startPosition.x - transform.position.x >= BOUNDARY/2)
		{
			if (canThrowSecondBoomerang == true)
			{
				if (isHoldBoomerang == false)
				{
					OnHoldBoomerang(normal);
					isHoldBoomerang = true;
					canAttack = false;
				}
			}
		}
	}
}

void CBoomerangBrother::OnHoldBoomerang(D3DXVECTOR2 normal)
{
	
	if (onHoldObject == NULL)
	{
		onHoldObject = boomerangs.Init();

		if (onHoldObject == NULL)
			return;

		auto currentBoomerang = static_cast<CBoomerang*>(onHoldObject);
		D3DXVECTOR2 pos = currentBoomerang->GetPosition();
		auto boomPhyBody = currentBoomerang->GetPhysiscBody();

		auto posBoomerangBrother = transform.position + relativePositionOnScreen;
		currentBoomerang->SetPosition(posBoomerangBrother);
		currentBoomerang->SetStartPosition(posBoomerangBrother);
		currentBoomerang->Enable(true);
	}
}

void CBoomerangBrother::OnDie()
{
	auto normal = physiscBody->GetNormal();
	normal.y = -normal.y;
	physiscBody->SetNormal(normal);
	this->collisionBoxs->at(0)->SetEnable(false);
}


