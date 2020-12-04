#include "MushroomEffect.h"
#include "AnimationManager.h"
#include "EffectConst.h"
#include "MushroomConst.h"
#include "Ultis.h"
CMushroomEffect::CMushroomEffect()
{
	LoadAnimation();
	SetState(MUSHROOM_EFFECT);
	SetScale(D3DXVECTOR2(0.4f, 1.0f));
	SetTag(GameObjectTags::SuperMushroom);
	itemTag = ItemTag::PowerUp;
	powerupTag = PowerupTag::SuperMushroom;
	isEnabled = true;

	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(MUSHROOM_BBOX);
	box->SetGameObjectAttach(this);
	box->SetEnable(true);
	this->collisionBoxs->push_back(box);
	physiscBody->SetGravity(MUSHROOM_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	physiscBody->SetDynamic(true);

	appearingCounting = 0;
}

void CMushroomEffect::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation(MUSHROOM_EFFECT, animations->Get("ani-super-mushroom"));
}

void CMushroomEffect::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	if (isAppearing == true)
	{

		velocity.y = -MUSHROOM_PUSH_FORCE;
		if (abs(startPosition.y) - abs(transform.position.y) > GROW_MAX_HEIGHT)
			isAppearing = false;
	}
	else
	{
		appearingCounting++;
		if (appearingCounting == 1)
			normal.x = marioFacing;
		velocity.x = normal.x * MUSHROOM_SPEED;
	}
	physiscBody->SetVelocity(velocity);
}

void CMushroomEffect::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}

void CMushroomEffect::StartEffect(float MarioFacing)
{
	isAppearing = true;
	this->marioFacing = MarioFacing;
}

bool CMushroomEffect::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (isAppearing == true && ( StaticTag(tag) == true || tag == GameObjectTags::GhostPlatform))
		return false;
	if (StaticTag(tag) == true || MarioTag(tag) == true || tag == GameObjectTags::GhostPlatform)
		return true;
	return false;
}
