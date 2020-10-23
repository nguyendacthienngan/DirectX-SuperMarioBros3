#include "SmallMario.h"
#include "MarioConst.h"
#include "MarioControllerConst.h"
#include "AnimationManager.h"
void CSmallMario::Init()
{
	SetTag(GameObjectTags::SmallMario);
	LoadAnimation();

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(SMALL_MARIO_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Mario"); // SmallMario
	this->collisionBoxs->push_back(collisionBox);
	canCrouch = false;
}

void CSmallMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-small-mario-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-small-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-small-mario-run"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-small-mario-jump"));
}
