#include "GreenParaKoopa.h"
#include "ParaKoopaConst.h"
#include "AnimationManager.h"


CGreenParaKoopa::CGreenParaKoopa()
{
	isJump = false;
	physiscBody->SetVelocity(D3DXVECTOR2(KOOPA_SPEED, 0.0f));
	LoadAnimation();
	SetState(PARAKOOPA_STATE_FLY);

}

void CGreenParaKoopa::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(PARAKOOPA_STATE_FLY, animationManager->Clone("ani-green-koopa-paratroopa-fly"));
}

void CGreenParaKoopa::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	velocity.x = normal.x * KOOPA_SPEED;

	if (isJump == false && IsOnGround() == true)
	{
		if (abs(jumpStartPosition) - abs(transform.position.y) <= PARA_KOOPA_HEIGHT)
		{
			jumpStartPosition = transform.position.y;
			velocity.y = -PARA_KOOPA_FORCE_Y;
			isJump = true;
		}
	}

	if (isJump == true)
	{
		isOnGround = false;

		if (abs(jumpStartPosition) - abs(transform.position.y) > PARA_KOOPA_HEIGHT)
		{
			velocity.y = PARA_KOOPA_FORCE_Y / 2;
		}
		if (velocity.y > 0)
		{
			isJump = false;
		}
	}
	physiscBody->SetVelocity(velocity);
}