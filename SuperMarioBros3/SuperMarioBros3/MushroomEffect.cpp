#include "MushroomEffect.h"
#include "AnimationManager.h"
#include "EffectConst.h"
#include "MushroomConst.h"
CMushroomEffect::CMushroomEffect()
{
	LoadAnimation();
	SetState(MUSHROOM_EFFECT);
	SetScale(D3DXVECTOR2(0.4f, 1.0f));
	SetTag(GameObjectTags::SuperMushroom);
	itemTag = ItemTag::SuperMushroom;
	isEnabled = true;

	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(MUSHROOM_BBOX);
	box->SetGameObjectAttach(this);
	box->SetEnable(true);
	this->collisionBoxs->push_back(box);
	physiscBody->SetGravity(MUSHROOM_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(MUSHROOM_SPEED, 0.0f));
	physiscBody->SetDynamic(true);
}

void CMushroomEffect::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation(MUSHROOM_EFFECT, animations->Get("ani-super-mushroom"));
}

void CMushroomEffect::Update(DWORD dt, CCamera* cam)
{
	auto normal = physiscBody->GetNormal();
	auto velocity = physiscBody->GetVelocity();
	velocity.x = normal.x * MUSHROOM_SPEED;
	physiscBody->SetVelocity(velocity);
}

void CMushroomEffect::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}

void CMushroomEffect::StartEffect()
{
	transform.position.y -= 60;
}

bool CMushroomEffect::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (StaticTag(tag) == true || MarioTag(tag) == true || tag == GameObjectTags::GhostPlatform)
		return true;
	return false;
}
