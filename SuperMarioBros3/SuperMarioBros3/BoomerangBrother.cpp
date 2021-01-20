#include "BoomerangBrother.h"
#include "AnimationManager.h"
#include "BoomerangBrotherConst.h"
#include "Boomerang.h"
#include "SceneManager.h"

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

	moveState = 0;
	isAttack = false;
	canAttack = false;
	timer = 0;
}

void CBoomerangBrother::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(BOOMERANG_STATE_MOVE, animationManager->Clone("ani-boomerang-brother-move"));
	AddAnimation(BOOMERANG_STATE_ATTACK, animationManager->Clone("ani-boomerang-brother-attack"));
}

void CBoomerangBrother::Render(CCamera* cam, int alpha)
{
	auto normal = physiscBody->GetNormal();
	SetScale(D3DXVECTOR2(-normal.x, 1.0f));

	if (isAttack == true)
		SetState(BOOMERANG_STATE_ATTACK);
	CGameObject::Render(cam, alpha);
}

void CBoomerangBrother::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto normal = physiscBody->GetNormal();
	auto velocity = physiscBody->GetVelocity();
	auto targetPos = D3DXVECTOR2(0,0);
	if (target != NULL)
	{
		normal.x = (target->GetPosition().x < this->transform.position.x) ? -1 : 1;
		targetPos = target->GetPosition();
	}
	physiscBody->SetNormal(normal);
	switch (moveState)
	{
		case 0:
		{
			// Đứng yên, xác định mario
			// Nhảy
			auto distanceBetweenTargetAndBoomerang = target->GetPosition().x - this->transform.position.x;
			if (abs(distanceBetweenTargetAndBoomerang) <= DISTANCE_CAN_THROW_TWICE)
				canThrowSecondBoomerang = true;
			DebugOut(L"Pos X %f \n", transform.position.x);
			timer += dt;
			if (timer > 1000)
			{
				moveState = 1;
				timer = 0;
			}
			canAttack = false;
			break;
		}
		case 1:
		{
			// Tiến về mario. Sau khi tiến xong thì đó là cột mốc X
			velocity.x = BOOMERANG_BROTHER_VELOCITY * normal.x;
			transform.position.x += velocity.x * dt;

			if (transform.position.x >= startPosition.x + BOUNDARY)
			{
				moveState = 2;
				startPosition.x = transform.position.x;
			}
			// Xét biên

			break;
		}
		case 2:
		{
			// Lùi
			canAttack = true;
			velocity.x = -BOOMERANG_BROTHER_VELOCITY * normal.x;
			transform.position.x += velocity.x * dt;

			// Xét biên
			if (transform.position.x <= startPosition.x - BOUNDARY)
			{
				startPosition.x = transform.position.x;
				if (canThrowSecondBoomerang == true)
					moveState = 3;
				else
					moveState = 0;
				canAttack = false;
			}
			
			break;
		}
		case 3:
		{
			// Tiến, quay về vị trí ban đầu (cột mốc X)
			canAttack = true;
			velocity.x = BOOMERANG_BROTHER_VELOCITY * normal.x;
			transform.position.x += velocity.x * dt;

			if (transform.position.x >= startPosition.x + BOUNDARY)
			{
				moveState = 4;
				startPosition.x = transform.position.x;
				canAttack = false;
			}

			break;
		}
		case 4:
		{
			// Lùi
			// Đang bị lõi chưa đc reset
			velocity.x = -BOOMERANG_BROTHER_VELOCITY * normal.x;
			if (transform.position.x <= startPosition.x - BOUNDARY)
			{
				startPosition.x = transform.position.x;
				moveState = 0;
				canAttack = false;
			}
			break;
		}
	}

	if (canAttack == true)
	{
		// Nếu có lấy boomerang được thì mới attack. Attack xong chuyển lại animation move
		
		if (isAttack == false)
		{
			auto currentObj = boomerangs.Init();
			if (currentObj != NULL)
			{
				isAttack = true;
				auto currentBoomerang = static_cast<CBoomerang*>(currentObj);
				D3DXVECTOR2 pos = currentBoomerang->GetPosition();
				currentBoomerang->SetPosition(transform.position);
				currentBoomerang->SetGoalPosition(targetPos);

				auto boomPhyBody = currentBoomerang->GetPhysiscBody();
				boomPhyBody->SetGravity(0.0f);

				auto posBoomerangBrother = transform.position + relativePositionOnScreen;
				posBoomerangBrother.x += BOOMERANG_BROTHER__BBOX.x * 0.5f * normal.x;
				currentBoomerang->SetPosition(posBoomerangBrother);
				currentBoomerang->SetStartPosition(posBoomerangBrother);
				currentBoomerang->Enable(true);

				auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
				auto grid = activeScene->GetGrid();
				if (grid != NULL && activeScene->IsSpacePartitioning() == true)
					grid->Move(pos, currentBoomerang);
			}

		}
	}
	else
		isAttack = false; // Chưa đúng lắm, khi nào tắt k attack nữa? K lẽ cho timer
	DebugOut(L"Move state %d \n", moveState);
}

CObjectPool CBoomerangBrother::GetObjectPool()
{
	return boomerangs;
}
