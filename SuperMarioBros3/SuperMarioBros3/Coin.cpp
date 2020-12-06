#include "Coin.h"
#include "AnimationManager.h"
#include "CoinConst.h"
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
	this->physiscBody->SetDynamic(false);
}

bool CCoin::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	return false;
}

void CCoin::SetType(int type)
{
	this->type = type;
}

int CCoin::GetType()
{
	return type;
}

