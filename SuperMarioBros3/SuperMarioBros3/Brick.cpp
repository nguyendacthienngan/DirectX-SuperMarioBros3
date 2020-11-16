#include "Brick.h"
#include "AnimationManager.h"
#include "BrickConst.h"

CBrick::CBrick()
{
	LoadAnimation();
	Init();
	SetState("BRICK");
	isEnabled = true;
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
	this->physiscBody->SetDynamic(false);
}

