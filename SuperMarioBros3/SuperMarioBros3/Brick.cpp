#include "Brick.h"
#include "AnimationManager.h"
#include "BrickConst.h"
#include "SceneManager.h"
#include "BrickEffect.h"

CBrick::CBrick()
{
	LoadAnimation();
	Init();
	SetState("BRICK");
	isEnabled = true;

	for (int i = 0; i < 4; i++)
	{
		CBrickEffect* brickFX = new CBrickEffect();
		brickFX->LinkToPool(&brickPool);
		brickPool.Add(brickFX);
	}
}

void CBrick::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("BRICK", animations->Get("ani-brick"));
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

void CBrick::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::RaccoonTail)
	{
		Debris();
		// Apply Effect Brick Breaking
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
	const float velx[4] = { +0.1875f, +0.25f, -0.25f, -0.1875f };
	const float vely[4] = { -0.375f, -0.75f, -0.75f, -0.375f };


	for (int i = 0; i < 4; i++)
	{
		auto brickFX = brickPool.Init();
		brickFX->GetPhysiscBody()->SetVelocity(D3DXVECTOR2(velx[i], vely[i]));
		brickFX->Enable(true);
		brickFX->SetPosition(this->GetPosition());
	}
	isEnabled = false;
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene != NULL)
	{
		activeScene->RemoveBrick(this);
		activeScene->RemoveObject(this);
		activeScene->AddDestroyObject(this);
	}
}

CObjectPool CBrick::GetObjectPool()
{
	return brickPool;
}

