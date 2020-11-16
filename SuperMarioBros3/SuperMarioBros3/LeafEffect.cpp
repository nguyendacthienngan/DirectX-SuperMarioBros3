#include "LeafEffect.h"
#include "AnimationManager.h"
#include "EffectConst.h"
#include "Ultis.h"

CLeafEffect::CLeafEffect()
{
	LoadAnimation();
	SetState(SUPER_LEAF_FALL_EFFECT);
	isEnabled = true;

	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(D3DXVECTOR2(0.0f, 0.0f));
	box->SetGameObjectAttach(this);
	box->SetEnable(false);
	this->collisionBoxs->push_back(box);
	physiscBody->SetGravity(LEAF_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, -LEAF_BOUNCE_FORCE));
	physiscBody->SetDynamic(true);

	isBounce = false;
	isFall = false;

	startFallTime = 0;
	SetScale(D3DXVECTOR2(0.4f, 1.0f));
}

void CLeafEffect::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation(SUPER_LEAF_FALL_EFFECT, animations->Get("ani-super-leaf-red"));
}

void CLeafEffect::Update(DWORD dt, CCamera* cam)
{
	// Chiếc lá khi tưng lên 1 khoảng vy
	auto vel = physiscBody->GetVelocity();

	if (isBounce == true && isFall == false)
	{
		DebugOut(L"LEAF UPDATE BOUNCE \n");

		if (abs(startPosition.y) - abs(transform.position.y) <= BOUNCE_MAX_HEIGHT)
		{
			physiscBody->SetVelocity(D3DXVECTOR2(0.0f, -LEAF_BOUNCE_FORCE));
		}
		else
		{
			isBounce = false;
			isFall = true;
			//startFallTime = GetTickCount64();
		}
	}
	if (isFall == true && isBounce == false)
	{
		DebugOut(L"LEAF FALL \n");
		physiscBody->SetVelocity(D3DXVECTOR2(0.0f, LEAF_BOUNCE_FORCE/2));
		// Chiếc lá khi rơi xuống sẽ rơi theo dao động điều hòa (Harmonic Motion)
		// Dùng công thức x = Acos(omega*t) với omega = 2 * pi * f
		// A là biên độ dao động, f là tần số

		int time = GetTickCount64() - startFallTime;
		transform.position.x = startPosition.x + LEAF_AMPLITUDE * cos(LEAF_SPEED * time);

		// Tính vctor để scale
		float relativeVel = -2 * LEAF_SPEED * LEAF_AMPLITUDE * sin(LEAF_SPEED * time);
		auto normal = physiscBody->GetNormal();
		normal.x = relativeVel >= 0 ? -1 : 1;
		physiscBody->SetNormal(normal);
	}
}

void CLeafEffect::Render(CCamera* cam, int alpha)
{
	SetScale(D3DXVECTOR2(physiscBody->GetNormal().x, 1.0f));
	CGameObject::Render(cam, alpha);
}

void CLeafEffect::StartEffect()
{
	isBounce = true;
	startFallTime = GetTickCount64();

}
