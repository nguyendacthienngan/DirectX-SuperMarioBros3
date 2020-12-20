#include "GameObject.h"
#include "GameObjectConst.h"
#include "Ultis.h"
#include "SceneManager.h"
#include "ScoreEffect.h"
using namespace std;

CGameObject::CGameObject()
{
	this->currentState = "";
	tag = GameObjectTags::None;
	physiscBody = new CPhysicsBody();
	collisionBoxs = new vector<CCollisionBox*>();
	isEnabled = false;
	ignoreTimeScale = false;
	currentState = "IDLE";
	isCheckWithCollision = true;
	nodeTag = NodeTag::None;
}

CGameObject::~CGameObject()
{
	if (physiscBody != NULL) delete physiscBody;
	for (auto col : *collisionBoxs)
		delete col;
	collisionBoxs->clear();
	delete collisionBoxs;
}


void CGameObject::Init()
{
}

bool CGameObject::IsDestroyed()
{
	return isDestroyed;
}

void CGameObject::SetDestroy(bool isDes)
{
	this->isDestroyed = isDes;
}

void CGameObject::Clear()
{
	
}

void CGameObject::LoadAnimation()
{
}

void CGameObject::PhysicsUpdate(std::vector<LPGameObject>* coObjects)
{
	if (physiscBody->IsDynamic() == false) return;
	if (StaticTag(tag) == false && isEnabled == false)
		return;
	vector<CCollisionBox*> otherCollisionBoxs;
	for (auto obj : *coObjects)
	{
		auto collisionBoxsOther = obj->GetCollisionBox();
		for (auto collisionBox : *collisionBoxsOther)
			otherCollisionBoxs.push_back(collisionBox);
	}

	for (auto collisionBox : *collisionBoxs)
	{
		physiscBody->Update(this);
		physiscBody->PhysicsUpdate(collisionBox, &otherCollisionBoxs);
	}

	ResetTempValues();
}

void CGameObject::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene->IsLoaded() == false)
		return;
}

void CGameObject::Render(CCamera* cam, int alpha)
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene->IsLoaded() == false)
		return;
	bool curState = animations.find(currentState) != animations.end();
	if (curState == false || animations.empty())
		return;
	animations.find(currentState)->second->SetScale(transform.scale);
	animations.find(currentState)->second->SetRotation(transform.rotationAngle);

	D3DXVECTOR2 posInCam = cam->Transform(transform.position + relativePositionOnScreen);
	posInCam.x = trunc(posInCam.x);
	posInCam.y = trunc(posInCam.y) + Small_Player_Distance;
	animations.at(currentState)->Render(posInCam, alpha);
}

void CGameObject::FrictionProcess(float& speed, DWORD dt, bool isStop)
{
	if (speed > 0)
	{
		speed += -(isStop ? ACCELERATION_FRICTION *2.5f : ACCELERATION_FRICTION) * dt;
		if (speed < 0)
			speed = 0;
	}
	if (speed < 0)
	{
		speed += (isStop ? ACCELERATION_FRICTION * 2.5f : ACCELERATION_FRICTION) * dt;
		if (speed > 0)
			speed = 0;
	}
}

void CGameObject::ResetTempValues()
{
	physiscBody->SetAcceleration(0.0f);
}


void CGameObject::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{

}

void CGameObject::OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{
}

void CGameObject::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
}

void CGameObject::OnDie()
{
}

void CGameObject::OnDamaged()
{
}

void CGameObject::OnScoreEffect()
{
	CScoreEffect* scoreFX = new CScoreEffect();
	scoreFX->SetStartPosition(transform.position);
	scoreFX->Enable(true);
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene)
		activeScene->AddObject(scoreFX);
}

std::string CGameObject::GetCurrentState()
{
	return currentState;
}

void CGameObject::AddAnimation(std::string stateName, LPAnimation animation, bool isLoop)
{
	animation->SetLoopAnimation(isLoop);
	animation->SetGameObject(this);
	animations.insert(make_pair(stateName, animation));
}

void CGameObject::SetRelativePositionOnScreen(D3DXVECTOR2 rP)
{
	this->relativePositionOnScreen = rP;
}

void CGameObject::EndAnimation()
{
}

void CGameObject::KeyState()
{
}

void CGameObject::OnKeyDown(int KeyCode)
{
}

void CGameObject::OnKeyUp(int KeyCode)
{
}

bool CGameObject::IsEnabled()
{
	return isEnabled;
}

void CGameObject::Enable(bool isEnabled)
{
	this->isEnabled = isEnabled;
}

bool CGameObject::IsIgnoreTimeScale()
{
	return ignoreTimeScale;
}

void CGameObject::SetIgnoreTimeSCale(bool isIgnoreTimeScale)
{
	this->ignoreTimeScale = isIgnoreTimeScale;
}

D3DXVECTOR2 CGameObject::GetScale()
{
	return transform.scale;
}

void CGameObject::SetScale(D3DXVECTOR2 s)
{
	this->transform.scale = s;
}

float CGameObject::GetRotation()
{
	return transform.rotationAngle;
}

void CGameObject::SetRotation(float r)
{
	this->transform.rotationAngle = r;
}

D3DXVECTOR2 CGameObject::GetPosition()
{
	return this->transform.position;
}

void CGameObject::SetPosition(D3DXVECTOR2 p)
{
	this->transform.position = p;
}

LPPhysicsBody CGameObject::GetPhysiscBody()
{
	return physiscBody;
}

void CGameObject::SetPhysiscBody(LPPhysicsBody p)
{
	this->physiscBody = p;
}

std::vector<LPCollisionBox>* CGameObject::GetCollisionBox()
{
	return collisionBoxs;
}

void CGameObject::GetCollisionBox(std::vector<LPCollisionBox>* listCollisionBox)
{
	this->collisionBoxs = listCollisionBox;
}

std::string CGameObject::GetState()
{
	return currentState;
}

void CGameObject::SetState(string state)
{
	if (animations.find(state) == animations.end())
		return;
	lastState = currentState;
	currentState = state;
	animations.at(state)->SetPlay(false);
}

GameObjectTags CGameObject::GetTag()
{
	return tag;
}

void CGameObject::SetTag(GameObjectTags t)
{
	this->tag = t;
}

void CGameObject::AddObjectToScene(LPScene scene)
{

}

bool CGameObject::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	return true;
}

bool CGameObject::MarioTag(GameObjectTags tag)
{
	if (tag == GameObjectTags::Player || tag == GameObjectTags::SmallPlayer)
		return true;
	return false;
}

bool CGameObject::StaticTag(GameObjectTags tag)
{
	if (tag == GameObjectTags::Solid || tag == GameObjectTags::QuestionBlock || tag == GameObjectTags::Brick)
		return true;
	return false;
}

bool CGameObject::GiftTag(GameObjectTags tag)
{
	if (tag == GameObjectTags::Gift || tag == GameObjectTags::SuperMushroom || tag == GameObjectTags::Coin)
		return true;
	return false;
}

bool CGameObject::PlayerAttackItemTag(GameObjectTags tag)
{
	if (tag == GameObjectTags::RaccoonTail || tag == GameObjectTags::MarioFireBall) // And Koopa Shell 
		return true;
	return false;
}


LPAnimation CGameObject::GetAnimationByState(std::string state)
{
	if (animations.find(state) != animations.end())
		return animations.at(state);
	return NULL;
}
