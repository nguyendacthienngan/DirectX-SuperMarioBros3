#include "SmallMario.h"
#include "MarioConst.h"
#include "MarioControllerConst.h"
#include "AnimationManager.h"
#include "Ultis.h"
CSmallMario::CSmallMario()
{
	CSmallMario::Init();
	CMario::InitProperties();
	tag = GameObjectTags::SmallPlayer;
	marioStateTag = MarioStates::SmallMario;

	canCrouch = false;
	canAttack = false;
}
void CSmallMario::Init()
{
	LoadAnimation();
	CCollisionBox* collisionBox = new CCollisionBox(); 
	collisionBox->SetSizeBox(SMALL_MARIO_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Mario"); // SmallMario
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);
}

void CSmallMario::LoadAnimation()
{
	DebugOut(L"Load animation Small Mario \n");
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-small-mario-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-small-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-small-mario-run"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-small-mario-jump"));
	AddAnimation(MARIO_STATE_FALL, animationManager->Get("ani-small-mario-jump"));
	AddAnimation(MARIO_STATE_CROUCH, animationManager->Get("ani-small-mario-idle"));
	CSmallMario::SetState(MARIO_STATE_IDLE);
}

CSmallMario::~CSmallMario()
{
	CGameObject::~CGameObject();

}
