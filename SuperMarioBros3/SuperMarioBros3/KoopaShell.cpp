#include "KoopaShell.h"
#include "KoopaConst.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "MiscConst.h"
CKoopaShell::CKoopaShell()
{
	LoadAnimation();
	Init();
	enemyTag = EnemyTag::KoopaShell;
	isRun = false;
	canRun = false;
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
	//DebugOut(L"KoopaShell Position.y %f \n", transform.position.y);
	auto vel = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();

	if ((IsHolding() == true && physiscBody->GetGravity() != 0) || isRun == true)
		physiscBody->SetGravity(0);
	else 
		physiscBody->SetGravity(KOOPA_GRAVITY);

	if (canRun == true)
	{
		vel.x = KOOPA_SHELL_SPEED * normal.x;
		isRun = true;
	}
	else
		vel.x = 0.0f;

	vel.y = 0.0f; // để tránh rớt xuống
	physiscBody->SetVelocity(vel);
	//DebugOut(L"KoopaShell Gravity  %f \n", physiscBody->GetGravity());
	//DebugOut(L"KoopaShell Velocity  %f, %f \n", vel.x, vel.y);

}

void CKoopaShell::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Solid)
		{
			if (collisionEvent->nx != 0)
			{
				DebugOut(L"Bounce Wall");

				auto normal = physiscBody->GetNormal();
				normal.x = -normal.x;
				physiscBody->SetNormal(normal);
			}
		}
		else if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Misc && collisionBox->GetName().compare(FIRE_BALL_NAME) == 0)
		{
			// Nếu mai rùa bị đạn bắn là nó lật lại (-1) r bị văng đi khỏi ground lun : HEADSHOT
			if (collisionEvent->nx != 0)
			{
				CKoopaShell::OnDie();
			}
		}
		else if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Player)
		{
			// Mario đạp lên đầu

			if (collisionEvent->ny != 0)
			{
				auto v = physiscBody->GetVelocity();

				// nếu đạp lên đầu khi chưa chạy thì cho nó chạy lun
				if (isRun == false)
				{
					canRun = true;
					physiscBody->SetGravity(0.0f);
					v.y = 0.0f;
					physiscBody->SetVelocity(v);
				}
				else
				{
					DebugOut(L"STOP RUN \n");
					isRun = false;
					canRun = false;

				}
			}
		}
	}
}

void CKoopaShell::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::RaccoonTail)
	{
		// chỉ khi bị đuôi quật nó mới set lại -1 r văng đi (chưa văng khỏi ground)
		// cần xử lý lại việc chết cho hợp lý
		//DebugOut(L"Koopa Shell Died \n");
		CKoopaShell::OnDie();
	}
	// Còn bình thường thì sao?
}

void CKoopaShell::OnDie()
{
	auto normal = physiscBody->GetNormal();
	normal.y = -1;
	physiscBody->SetNormal(normal);

	// Chỗ này giúp tạo hiệu ứng văng đi => Và có thể giúp k bị rớt khỏi mặt đất
	/*auto v = physiscBody->GetVelocity();
	v.y = -1.0f;
	physiscBody->SetVelocity(v);*/
	physiscBody->SetGravity(0.0f);
	transform.position.y -= 1;
}

void CKoopaShell::SetHoldablePosition(D3DXVECTOR2 pos)
{
	SetPosition(pos);
}

void CKoopaShell::Release()
{
	CHoldable::Release();
	// Trả lại chuyển động bình thường
	isEnabled = true;
	physiscBody->SetNormal(normal); // Sẽ set lại normal theo hướng bị thả ra dựa vào normal bên class cha Holdable giữ
}

D3DXVECTOR2 CKoopaShell::GetHoldableCollisionBox()
{
	return collisionBoxs->at(0)->GetSizeBox();
}

void CKoopaShell::Render(CCamera* cam)
{
	auto normal = physiscBody->GetNormal();
	SetScale(normal);
	CGameObject::Render(cam);
}

bool CKoopaShell::IsRunning()
{
	return isRun || canRun;
}

void CKoopaShell::SetRun(bool canRun)
{
	this->canRun = canRun;
}
