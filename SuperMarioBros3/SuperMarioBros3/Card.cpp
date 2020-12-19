#include "Card.h"
#include "AnimationManager.h"
#include "UICamera.h"
#include "Mario.h"
#include "SceneManager.h"

CCard::CCard()
{
	LoadAnimation();
	CCollisionBox* collisionBox = new CCollisionBox();
	collisionBox->SetSizeBox(D3DXVECTOR2(48,48));
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Card");
	collisionBox->SetPosition(D3DXVECTOR2(0.0f, 0.0f));

	this->collisionBoxs->push_back(collisionBox);
	physiscBody->SetDynamic(true);
	physiscBody->SetGravity(0.0f);
	isEnabled = true;	
	cardState = 0;
	isTouched = false;
	goalFX = NULL;
	tag = GameObjectTags::Card;
}

void CCard::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("FLOWER", animationManager->Clone("ani-fire-flower"));
	AddAnimation("STAR", animationManager->Clone("ani-star-man-0"));
	AddAnimation("MUSHROOM", animationManager->Clone("ani-super-mushroom"));
}

void CCard::SetCardState(int state)
{
	this->cardState = state;
}

void CCard::Render(CCamera* cam, int alpha)
{
	if (cardState == 0)
		SetState("FLOWER");
	if (cardState == 5)
		SetState("STAR");
	if (cardState == 10)
		SetState("MUSHROOM");
	if (isTouched == true)
		alpha = 0;
	CGameObject::Render(cam, alpha);
}

void CCard::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	if (isTouched == true)
	{
		//Khi nào xét FX bị disable thì mình disable card luôn
		if (goalFX != NULL)
			if (goalFX->IsEnabled() == false)
				this->Enable(false);
	}
	
}

void CCard::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{
	for (auto collisionEvent : otherCollisions)
	{
		auto collisionBox = collisionEvent->obj;
		if (MarioTag(collisionBox->GetGameObjectAttach()->GetTag()))
		{
			auto mario = static_cast<CMario*>(collisionBox->GetGameObjectAttach());
			mario->HitGoalRoulette();
			HitGoalRoulette();
		}
	}
}

void CCard::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	if (MarioTag(otherCollisionBox->GetGameObjectAttach()->GetTag()))
	{
		auto mario = static_cast<CMario*>(otherCollisionBox->GetGameObjectAttach());
		mario->HitGoalRoulette();
		HitGoalRoulette();
	}
}

bool CCard::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (MarioTag(tag))
		return true;
	return false;
}

void CCard::IsTouched(bool state)
{
	isTouched = state;
}

bool CCard::IsTouched()
{
	return isTouched;
}

void CCard::HitGoalRoulette()
{
	if (goalFX == NULL)
	{
		goalFX = new CGoalEffects();
		goalFX->SetStartPosition(this->GetPosition());
		goalFX->SetState(currentState);
		auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
		activeScene->AddObject(goalFX);
	}
	// Set State Goals FX
	isTouched = true;
}
