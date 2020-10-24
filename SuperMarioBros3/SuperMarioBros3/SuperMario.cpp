#include "SuperMario.h"
#include "AnimationManager.h"
#include "MarioConst.h"
#include "Ultis.h"
CSuperMario::CSuperMario()
{
	CSuperMario::LoadAnimation();
	CSuperMario::Init();
	CMario::InitProperties();
}
void CSuperMario::Init()
{
	
	this->SetState(MARIO_STATE_IDLE); // Để tên đồng nhất với animation

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(SUPER_MARIO_BBOX); // Big
	//collisionBox->SetSizeBox(BIG_MARIO_BBOX); // Big
	//collisionBox->SetSizeBox(D3DXVECTOR2(14 * 3, 27 * 3)); // Big
	collisionBox->SetPosition(D3DXVECTOR2(0.0f, 0.0f)); // Local Position

	//collisionBox->SetSizeBox(D3DXVECTOR2(12 * 3, 15 * 3)); // Small
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Mario");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	CSuperMario::collisionBoxs->push_back(collisionBox);
	//auto id = CSuperMario::collisionBoxs->at(0)->GetID();
	//DebugOut(L"Init Super Mario . Collision Box id : \n", id);

}
void CSuperMario::LoadAnimation()
{
	DebugOut(L"Init Super Mario \n");
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_IDLE, animationManager->Get("ani-big-mario-idle"));
	AddAnimation(MARIO_STATE_WALKING, animationManager->Get("ani-big-mario-walk"));
	AddAnimation(MARIO_STATE_RUNNING, animationManager->Get("ani-big-mario-run"));
	AddAnimation(MARIO_STATE_HIGH_SPEED, animationManager->Get("ani-big-mario-high-speed"));
	AddAnimation(MARIO_STATE_JUMP, animationManager->Get("ani-big-mario-jump"));
	AddAnimation(MARIO_STATE_CROUCH, animationManager->Get("ani-big-mario-crouch"));
	AddAnimation(MARIO_STATE_SKID, animationManager->Get("ani-big-mario-skid"));
	AddAnimation(MARIO_STATE_FALL, animationManager->Get("ani-big-mario-fall"));
}
