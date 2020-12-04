#include "RedParaGoomba.h"
#include "AnimationManager.h"
#include "ParaGoombaConst.h"
CRedParaGoomba::CRedParaGoomba()
{
	LoadAnimation();
	Init();
	redGoomba = NULL;
}

void CRedParaGoomba::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(PARA_GOOMBA_IDLE, animationManager->Clone("ani-red-para-goomba-idle"));
	AddAnimation(PARA_GOOMBA_WALK, animationManager->Clone("ani-red-para-goomba-walk"));
	AddAnimation(PARA_GOOMBA_FLY, animationManager->Clone("ani-red-para-goomba-fly"));

}

void CRedParaGoomba::Init()
{
	SetState(PARA_GOOMBA_FLY);
	isEnabled = true;
	enemyTag = EnemyTag::ParaGoomba;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(GOOMBA_SIZEBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Para-Goomba");
	collisionBox->SetPosition(D3DXVECTOR2(0.0f, GOOMBA_SIZEBOX.y / 2 - DISTANCE_BETWEEN_PARA_GOOMBA_PIVOT_AND_GOOMBA_PIVOT));
	this->collisionBoxs->push_back(collisionBox);

	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(RED_PARAGOOMBA_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(RED_PARAGOOMBA_SPEED, 0.0f));
}

