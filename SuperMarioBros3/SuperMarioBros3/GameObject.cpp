#include "GameObject.h"

// Tạm thời
#include "Game.h"
#include "TextureManager.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

CGameObject::CGameObject()
{
	this->currentState = "";
	tag = GameObjectTags::None;
	physiscBody = new CPhysicsBody();
	collisionBoxs = new vector<CCollisionBox*>();
	isEnabled = false;
}

CGameObject::~CGameObject()
{
	for (auto a : animations)
		delete a.second;
}


void CGameObject::Init()
{
}

void CGameObject::LoadAnimation()
{
}

void CGameObject::PhysicsUpdate(std::vector<LPGameObject>* coObjects)
{
	if (physiscBody->IsDynamic() == false) return;

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
}

void CGameObject::Update(DWORD dt, CCamera* cam)
{
}

void CGameObject::Render(CCamera* cam)
{
	bool curState = animations.find(currentState) != animations.end();
	if (curState == false || animations.empty())
		return;
	animations.find(currentState)->second->SetScale(transform.scale);
	animations.find(currentState)->second->SetRotation(transform.rotationAngle);

	D3DXVECTOR2 posInCam = cam->Transform(transform.position + relativePositionOnScreen);
	posInCam.x = trunc(posInCam.x);
	//posInCam.y = trunc(posInCam.y);

	if (tag != GameObjectTags::SmallMario)
	{
		//posInCam.x = trunc(posInCam.x) - 40; 
		//posInCam.y = trunc(posInCam.y) - 20; 
		posInCam.y = trunc(posInCam.y) + 18; // Đang hardcode, sẽ fix sau


	}
	animations.at(currentState)->Render(posInCam);
}


void CGameObject::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{
}

void CGameObject::OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{
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

Effector CGameObject::GetEffector()
{
	return effector;
}

void CGameObject::SetEffector(Effector e)
{
	this->effector = effector;
}

void CGameObject::AddMiscToScene(LPScene scene)
{

}

LPAnimation CGameObject::GetAnimationByState(std::string state)
{
	if (animations.find(state) != animations.end())
		return animations.at(state);
	return NULL;
}
