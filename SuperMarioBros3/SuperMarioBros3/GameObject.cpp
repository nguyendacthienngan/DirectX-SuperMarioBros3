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
	//CGameObject::Init(); // chỗ này bị lỗi nếu để Init là thuần ảo. Ta phải để Init là ảo thôi để có thể gọi được ngay trong cha của nó

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

//void CGameObject::Update(DWORD dt, CCamera* cam)
//{
//	DebugOut(L"[INFO] Game Object Updating.. \n");
//
//}

void CGameObject::Render(CCamera* cam)
{
	bool curState = animations.find(currentState) != animations.end();
	if (curState == false || animations.empty())
		return;
	//DebugOut(L"Mario position: x,y %f, %f \n", transform.position.x, transform.position.y);
	animations.find(currentState)->second->SetScale(transform.scale);
	animations.find(currentState)->second->SetRotation(transform.rotationAngle);
	D3DXVECTOR2 posInCam = cam->Transform(transform.position + relativePositionOnScreen);
	posInCam.x = trunc(posInCam.x);
	posInCam.y = trunc(posInCam.y); 
	//posInCam.y = 289;
	/*DebugOut(L"Mario position: x,y %f, %f \n", transform.position.x, transform.position.y);
	DebugOut(L"Mario position in cam: x,y %f, %f \n", posInCam.x, posInCam.y);*/

	animations.at(currentState)->Render(posInCam);
}


void CGameObject::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{
}

void CGameObject::OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{
}

void CGameObject::AddAnimation(std::string stateName, LPAnimation animation)
{
	animations.insert(make_pair(stateName, animation));
}

void CGameObject::SetRelativePositionOnScreen(D3DXVECTOR2 rP)
{
	this->relativePositionOnScreen = rP;
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

void CGameObject::SetState(string state)
{
	currentState = state;
}

LPAnimation CGameObject::GetAnimationByState(std::string state)
{
	if (animations.find(state) != animations.end())
		return animations.at(state);
	return NULL;
}
