#include "Enemy.h"
#include "Ultis.h"
#include "Koopa.h"
#include "ParaKoopa.h"
#include "KoopaConst.h"
#include "ScoreEffect.h"
#include "SceneManager.h"
#include "QuestionBlock.h"
#include "Brick.h"

CEnemy::CEnemy()
{
	this->SetTag(GameObjectTags::Enemy);
	this->SetScale(D3DXVECTOR2(1.0f, 1.0f));
	countDeadCallback = 0;
	countBeated = 0;
	isHeadShot = false;
	isHeadShotByFireBall = false;
	hitFX = new CHitEffects();
	isStatic = false;
}

void CEnemy::SetEnemyType(std::string eT)
{
	enemyType = eT;
}

std::string CEnemy::GetEnemyType()
{
	return enemyType;
}

void CEnemy::SetEnemyTag(EnemyTag eT)
{
	enemyTag = eT;
}

EnemyTag CEnemy::GetEnemyTag()
{
	return enemyTag;
}

void CEnemy::SetStartPosition(D3DXVECTOR2 sT)
{
	this->startPosition = sT;
}

D3DXVECTOR2 CEnemy::GetStartPosition()
{
	return startPosition;
}

void CEnemy::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Solid || collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::GhostPlatform
			|| collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::QuestionBlock || collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Brick
			|| collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::SwitchBlock
			|| collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Pipe)
		{
			if (collisionEvent->ny < 0 && isOnGround == false)
			{
				isOnGround = true;
			}
			if (collisionEvent->nx != 0)
			{
				auto normal = physiscBody->GetNormal();
				normal.x = -normal.x;
				physiscBody->SetNormal(normal);
				if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::QuestionBlock && enemyTag == EnemyTag::KoopaShell)
				{
					auto questionBlock = static_cast<CQuestionBlock*>(collisionBox->GetGameObjectAttach());
					questionBlock->Bounce();
				}
				if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Brick && enemyTag == EnemyTag::KoopaShell)
				{
					auto brick = static_cast<CBrick*>(collisionBox->GetGameObjectAttach());
					brick->Bounce();
					brick->Debris();
				}
			}
			if (this->enemyTag == EnemyTag::Koopa)
			{
				auto koopa = static_cast<CKoopa*>(this);
				if (koopa->GetKoopaType() == KoopaType::Red)
				{
					if (collisionEvent->ny >= 0 || collisionEvent->nx != 0)
						continue;
					auto otherBBox = collisionBox->GetBoundingBox();
					auto normal = physiscBody->GetNormal();
					auto velocity = physiscBody->GetVelocity();
					if (transform.position.x > otherBBox.right)
					{
						normal.x = -1;
						velocity.x = -velocity.x;
						transform.position.x = otherBBox.right - 1;
					}
					if (transform.position.x < otherBBox.left)
					{
						transform.position.x = otherBBox.left + 1;
						normal.x = 1;
						velocity.x = -velocity.x;
					}
					physiscBody->SetNormal(normal);
					physiscBody->SetVelocity(velocity);
				}
			}
		}
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Enemy)
		{
			auto otherObject = collisionBox->GetGameObjectAttach();
			auto otherEnemyObject = static_cast<CEnemy*>(otherObject);
			if (collisionEvent->nx != 0)
			{
				// Nếu 2 quái cùng loại thì đổi chiều hướng đi
				// Khác loại thì xuyên qua nhưng khác loại mà 1 loại không phải mai rùa, 1 loại là mai rùa thì loại đó phải đi đổi chiều
				if (otherEnemyObject->GetEnemyTag() == this->enemyTag ||
					( this->enemyTag != EnemyTag::KoopaShell  && otherEnemyObject->GetEnemyTag() == EnemyTag::KoopaShell))
				{
					DebugOut(L"2 enemy gap nhau \n");

					auto normal = physiscBody->GetNormal();
					auto velocity = physiscBody->GetVelocity();
					normal.x = -normal.x;
					velocity.x *= normal.x;
					transform.position.x = velocity.x * dt;
					physiscBody->SetNormal(normal);
					physiscBody->SetVelocity(velocity);
				}
			}
			if (enemyTag == EnemyTag::KoopaShell)
			{
				auto koopaShell = static_cast<CKoopaShell*>(this);
				koopaShell->CollisionWithOtherEnemy(collisionEvent, collisionBox);
			}
		}
		else if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::MarioFireBall)
		{
			if (collisionEvent->nx != 0 || collisionEvent->ny != 0)
			{
				isHeadShot = true;
				OnScoreEffect();

				if (enemyTag == EnemyTag::Koopa || enemyTag == EnemyTag::ParaKoopa)
				{
					this->OnDamaged(collisionBox->GetGameObjectAttach());
				}
				else if (enemyTag == EnemyTag::KoopaShell)
				{
					auto koopaShell = static_cast<CKoopaShell*>(this);
					koopaShell->CollisionWithFireBall();
				}
				else
				{
					OnDie();
				}
			}
		}
	}
}

void CEnemy::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::RaccoonTail)
	{
		isHeadShot = true;
		OnScoreEffect();

		if (enemyTag == EnemyTag::Koopa || enemyTag == EnemyTag::ParaKoopa)
		{
			this->OnDamaged(otherCollisionBox->GetGameObjectAttach());
		}
		else if (enemyTag == EnemyTag::KoopaShell)
		{
			auto koopaShell = static_cast<CKoopaShell*>(this);
			koopaShell->CollisionWithRaccoonTail(otherCollisionBox->GetGameObjectAttach());
		}
		else
		{
			OnDie();
		}
	}
	else if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::MarioFireBall)
	{
		isHeadShot = true;
		OnScoreEffect();

		if (enemyTag == EnemyTag::Koopa || enemyTag == EnemyTag::ParaKoopa)
		{
			this->OnDamaged(otherCollisionBox->GetGameObjectAttach());
		}
		else if (enemyTag == EnemyTag::KoopaShell)
		{
			auto koopaShell = static_cast<CKoopaShell*>(this);
			koopaShell->CollisionWithFireBall();
		}
		else
		{
			OnDie();
		}
	}
}

void CEnemy::OnDamaged(CGameObject* otherGO)
{
}

bool CEnemy::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (tag == GameObjectTags::VenusFireBall || tag == GameObjectTags::Gift )
		return false;
	if (MarioTag(tag) == true) // Mario tự xét với Enemy, chứ Enemy k xét với Mario
		return false;
	if (GiftTag(tag) == true || tag == GameObjectTags::Coin)
		return false;
	if (tag == GameObjectTags::Enemy)
	{
		auto enemy = static_cast<CEnemy*>(gO);
		// Enemy khác loại thì k xét (Trừ với koopa shell)
		if (enemyTag != EnemyTag::KoopaShell && enemy->GetEnemyTag() != EnemyTag::KoopaShell)
		{
			if (enemyTag != enemy->GetEnemyTag())
				return false;
		}
		// Plant vs Static: Không xét
		if (enemyTag == EnemyTag::Piranha || enemyTag == EnemyTag::Venus)
		{
			if (StaticTag(tag) == true || tag == GameObjectTags::GhostPlatform)
				return false;
		}
	}
	return true;
}

void CEnemy::SetHitFX(CHitEffects* hitFX)
{
	this->hitFX = hitFX;
}

void CEnemy::SetIsHeadShot(bool isHeadShot)
{
	this->isHeadShot = isHeadShot;
}

void CEnemy::SetIsHeadShotByFireball(bool isHeadShotByFireball)
{
	this->isHeadShotByFireBall = isHeadShotByFireball;
}

void CEnemy::SetTarget(CGameObject* target)
{
	if (target == NULL)
		return;
	this->target = target;
}

void CEnemy::CollideUnderMario()
{
	if (enemyTag == EnemyTag::Piranha || enemyTag == EnemyTag::Venus)
		return;
	countBeated++;
	if (countBeated == 1)
		OnScoreEffect();
}
