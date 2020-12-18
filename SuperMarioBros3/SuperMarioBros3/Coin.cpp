#include "Coin.h"
#include "AnimationManager.h"
#include "CoinConst.h"
#include "SceneManager.h"
CCoin::CCoin()
{
	LoadAnimation();
	Init();
	SetState(COIN_STATE_SPIN);
	isEnabled = true;
}

void CCoin::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation(COIN_STATE_SPIN, animations->Get("ani-coin"));
}

void CCoin::Init()
{
	tag = GameObjectTags::Coin;
	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(COIN_BBOX);
	box->SetGameObjectAttach(this);
	this->collisionBoxs->push_back(box);
	this->physiscBody->SetDynamic(true);
	this->physiscBody->SetGravity(0.0f);
}

bool CCoin::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (MarioTag(tag))
		return true;
	return false;
}

void CCoin::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{
	//for (auto collisionEvent : otherCollisions)
	//{
	//	auto collisionBox = collisionEvent->obj;
	//	if (MarioTag(collisionBox->GetGameObjectAttach()->GetTag()))
	//	{
	//		// Đụng trúng tiền là tăng tiền và disbale tiền
	//		this->isEnabled = false;
	//		OnScoreEffect();
	//		auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	//		if (activeScene != NULL)
	//		{
	//			activeScene->RemoveCoin(this);
	//			activeScene->RemoveObject(this);
	//			activeScene->AddDestroyObject(this);
	//		}
	//	}
	//}
}

void CCoin::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (MarioTag(otherCollisionBox->GetGameObjectAttach()->GetTag()))
	{
		// Đụng trúng tiền là tăng tiền và disbale tiền
		this->isEnabled = false;
		OnScoreEffect();
		auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
		if (activeScene != NULL)
		{
			activeScene->RemoveCoin(this);
			activeScene->RemoveObject(this);
			activeScene->AddDestroyObject(this);
		}
	}
}

void CCoin::SetType(int type)
{
	this->type = type;
}

int CCoin::GetType()
{
	return type;
}

