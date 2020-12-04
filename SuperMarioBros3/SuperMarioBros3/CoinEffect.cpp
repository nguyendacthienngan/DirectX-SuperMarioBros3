#include "CoinEffect.h"
#include "AnimationManager.h"
#include "CoinConst.h"
#include "EffectConst.h"

CCoinEffect::CCoinEffect()
{
	LoadAnimation();
	SetState(COIN_STATE_SPIN);
	isEnabled = true;

	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(D3DXVECTOR2(0.0f, 0.0f));
	box->SetGameObjectAttach(this);
	box->SetEnable(false);
	this->collisionBoxs->push_back(box);
	physiscBody->SetGravity(COIN_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, -COIN_BOUNCE));
	physiscBody->SetDynamic(true);

	SetScale(D3DXVECTOR2(0.4f, 1.0f));

}

void CCoinEffect::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation(COIN_STATE_SPIN, animations->Get("ani-coin"));
}

void CCoinEffect::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	if (transform.position.y > startPosition.y && physiscBody->GetVelocity() > 0)
		isEnabled = false;
}
