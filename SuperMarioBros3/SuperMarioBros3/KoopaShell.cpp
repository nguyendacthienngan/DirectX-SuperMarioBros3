#include "KoopaShell.h"
#include "KoopaConst.h"
#include "AnimationManager.h"
#include "Ultis.h"
CKoopaShell::CKoopaShell()
{
	LoadAnimation();
	Init();
}

void CKoopaShell::Init()
{
	LoadAnimation();
	SetState(KOOPA_SHELL_STATE_IDLE);
	isEnabled = true;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(KOOPA_SHELL_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Koopa-Shell");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);


	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(KOOPA_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
}

void CKoopaShell::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(KOOPA_SHELL_STATE_IDLE, animationManager->Get("ani-red-koopa-troopa-shell-idle"));
	AddAnimation(KOOPA_SHELL_STATE_RUN, animationManager->Get("ani-red-koopa-troopa-shell-run"));
}

void CKoopaShell::Update(DWORD dt, CCamera* cam)
{
	DebugOut(L"KoopaShell Position.y %f \n", transform.position.y);
}

void CKoopaShell::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{

}
