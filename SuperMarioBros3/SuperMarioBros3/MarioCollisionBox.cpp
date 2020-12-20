#include "MarioCollisionBox.h"
#include "Enemy.h"
#include "KoopaShell.h"
#include "Koopa.h"
#include "MarioConst.h"
#include "Ultis.h"
#include "ParaKoopa.h"
#include "RedParaGoomba.h"
#include "PSwitch.h"

void CMarioCollisionBox::CollisionHandle(DWORD dt, std::vector<CollisionEvent*>& collisionEvents, LPPhysicsBody phyBody, D3DXVECTOR2 vel, int mintx, int minty, float nx, float ny)
{
	CCollisionBox::CollisionHandle(dt, collisionEvents, phyBody, vel, mintx, minty, nx, ny);
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Enemy)
		{
			if (collisionEvent->ny < 0) // nhảy lên đầu quái
			{
				if (mario->StopBounce() == false)
					mario->JumpProcess(-MARIO_DEFLECT_Y, true);

				auto otherObject = collisionBox->GetGameObjectAttach();
				auto otherEnemyObject = static_cast<CEnemy*>(otherObject);
				otherEnemyObject->CollideUnderMario();
				switch (otherEnemyObject->GetEnemyTag())
				{
					case EnemyTag::KoopaShell:
					{
						auto koopaShell = static_cast<CKoopaShell*>(otherObject);
						// Koopa Shell
						if (koopaShell->IsRunning() == false)
						{
							koopaShell->SetRun();
						}
						else if (collisionEvent->ny < 0)
						{
							koopaShell->SetStopRun();
						}
						break;
					}
					case EnemyTag::Goomba:
					{
						otherEnemyObject->OnDie();
						break;
					}
					case EnemyTag::Koopa:
					{
						auto koopa = static_cast<CKoopa*>(otherObject);
						koopa->ChangeToShell();
						break;
					}
					case EnemyTag::ParaKoopa:
					{
						auto paraKoopa = static_cast<CParaKoopa*>(otherObject);
						paraKoopa->ChangeToKoopa();
						break;
					}
					case EnemyTag::ParaGoomba:
					{
						auto paraGoomba = static_cast<CRedParaGoomba*>(otherObject);
						paraGoomba->ChangeToGoomba();
						break;
					}
					case EnemyTag::Venus: case EnemyTag::Piranha:
					{
						mario->OnDamaged();
						auto normal = mario->GetPhysiscBody()->GetNormal();
						mario->GetPhysiscBody()->SetVelocity(D3DXVECTOR2(MARIO_WALKING_SPEED * normal.x, 0.0f)); // Khi bị nhảy lên trúng Venus => bị damaged và đẩy ra
					}
				}
			}
			if (collisionEvent->ny > 0) // Nhảy từ dưới lên
			{
				// Bị damaged
				mario->OnDamaged();
			}
			if (collisionEvent->nx != 0)
			{
				auto otherObject = collisionBox->GetGameObjectAttach();
				auto otherEnemyObject = static_cast<CEnemy*>(otherObject);
				switch (otherEnemyObject->GetEnemyTag())
				{
				case EnemyTag::KoopaShell:
				{
					auto koopaShell = static_cast<CKoopaShell*>(otherObject);

					// Koopa Shell
					if (mario->CanRun() == true && koopaShell->IsRunning() == false) // Mario đang chạy
					{
						// Nếu mario đang chạy => Chạm mai rùa thì có thể cầm được mai rùa
						// Lúc sau có thể bị đè bởi sự kiện mà set state k đc hay k?
						if (koopaShell != NULL)
						{
							mario->HoldObject(koopaShell);
							koopaShell->GetPhysiscBody()->SetGravity(0.0f);
						}
					}
					else
					{
						if (koopaShell->IsRunning() == false) // mai rùa đang chạy thì k kick đc nữa mà mario bị damaged
						{
							mario->KickProcess(true);
							auto normal = koopaShell->GetPhysiscBody()->GetNormal();
							normal.x = mario->GetPhysiscBody()->GetNormal().x;
							koopaShell->GetPhysiscBody()->SetNormal(normal);
							koopaShell->SetRun();
						}
						else if (koopaShell->IsHolding() == false)
						{
							// Mario bị damaged
							mario->OnDamaged();
						}
					}
					break;
				}
				default:
				{
					// TO-DO: Bị damaged
					mario->OnDamaged();
					break;
				}
				}
			}
		}
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::VenusFireBall)
		{
			mario->OnDamaged();
			collisionBox->GetGameObjectAttach()->Enable(false);
		}
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::SwitchBlock)
		{
			if (collisionEvent->ny < 0)
			{
				auto switchBlock = static_cast<CPSwitch*>(collisionBox->GetGameObjectAttach());
				if (switchBlock->GetChangeState() == 0)
					switchBlock->Active();
			}
		}
	}
}

void CMarioCollisionBox::SetGameObjectAttach(LPGameObject gO)
{
	CCollisionBox::SetGameObjectAttach(gO);
	mario = dynamic_cast<CMario*>(gO);
}
