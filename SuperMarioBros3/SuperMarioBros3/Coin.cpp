#include "Coin.h"
#include "AnimationManager.h"
#include "CoinConst.h"
CCoin::CCoin()
{
	LoadAnimation();
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
	tag = GameObjectTags::Solid;
	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(COIN_BBOX);
	box->SetGameObjectAttach(this);
	this->collisionBoxs->push_back(box);
	this->physiscBody->SetDynamic(false);
}
