#include "FireBall.h"
#include "MiscConst.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "SceneManager.h"
#include "FireMario.h"

CFireBall::CFireBall()
{
	this->SetTag(GameObjectTags::Misc);

	LoadAnimation();
	SetState(FIRE_BALL_ANIMATION);
	isEnabled = false;

	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(FIRE_BALL_BBOX);
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Fire-Ball"); 
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);


	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(FIRE_BALL_GRAVITY);
	physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	physiscBody->SetBounceForce(FIRE_BALL_BOUNCE_FORCE);
}

void CFireBall::PhysicsUpdate(std::vector<LPGameObject>* coObjects)
{

	CGameObject::PhysicsUpdate(coObjects);
}

void CFireBall::Update(DWORD dt, CCamera* cam)
{
	CGameObject::Update(dt, cam);

}

void CFireBall::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(FIRE_BALL_ANIMATION, animationManager->Get("ani-fire-ball"));

}

void CFireBall::SetFireMario(CGameObject* fireM)
{
	fireMario = fireM;
}

void CFireBall::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Solid
			&& collisionEvent->nx != 0)
		{
			this->isEnabled = false;
			auto scene = CSceneManager::GetInstance()->GetActiveScene();
			scene->RemoveObject(this);
		}
	}
}

CGameObject* CFireBall::GetFireMario()
{
	return fireMario;
}
