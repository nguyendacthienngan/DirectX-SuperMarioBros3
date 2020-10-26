#include "RacoonMario.h"
#include "MarioConst.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "MarioStateSet.h"
CRacoonMario::CRacoonMario()
{
	CMario::Init();
	SetTag(GameObjectTags::RaccoonMario);
	canCrouch = true;
	canAttack = true;
	CRacoonMario::Init();
	CRacoonMario::LoadAnimation();
}

void CRacoonMario::Init()
{
	SetState(MARIO_STATE_IDLE);
}

void CRacoonMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();

	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-raccoon-mario-idle"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-raccoon-mario-walk"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-raccoon-mario-walk"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-raccoon-mario-jump"));
	AddAnimation(MARIO_STATE_FLY, animationManager->Get("ani-raccoon-mario-fly"));
	AddAnimation(MARIO_STATE_FALL, animationManager->Get("ani-raccoon-mario-fall"));
	AddAnimation(MARIO_STATE_SKID, animationManager->Get("ani-raccoon-mario-skid"));
	AddAnimation(MARIO_STATE_CROUCH, animationManager->Get("ani-raccoon-mario-crouch"));
	AddAnimation(MARIO_STATE_ATTACK, animationManager->Get("ani-raccoon-mario-spin"), false);
}

void CRacoonMario::EndAnimation()
{
	if (currentState.compare(MARIO_STATE_ATTACK) == 0)
	{
		isAttack = false;
		if (animations.find(lastState) == animations.end()) // Không kiếm được last state trong animation, đồng nghĩa với việc last state chưa được khởi tạo, còn nếu đc khởi tạo rồi thì mình set state theo cái state trước đó
			lastState = MARIO_STATE_IDLE;
		SetState(lastState);
	}
}

void CRacoonMario::UpdatePositition()
{
	float pointCenterX = 0;
	if (tag == GameObjectTags::RaccoonMario)
	{
		DebugOut(L"Normal Mario x: %f \n", physiscBody->GetNormal().x);
		if (physiscBody->GetNormal().x == 1)
		{
			auto animation = animations.at(currentState);
			if (animation == NULL)
				DebugOut(L"Animation is NULL \n");
			else
			{
				auto sprite = animation->GetAnimFrame()->GetSprite();
				if (sprite == NULL)
					DebugOut(L"Sprite is NULL \n");
				else
				{
					pointCenterX = sprite->GetPointCenter().x;
					DebugOut(L"Point Center X: %f \n", pointCenterX);
				}

			}
		}
	}
}

void CRacoonMario::SetPositionToPointCenter(bool isPointCenter)
{
	isSetToPointCenter = isPointCenter;
}
