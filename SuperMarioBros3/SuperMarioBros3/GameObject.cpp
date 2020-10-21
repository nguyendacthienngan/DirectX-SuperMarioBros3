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
	tag = "";
	physiscBody = new CPhysicsBody();
	collisionBoxs = new vector<CCollisionBox*>();
	isEnabled = false;
	//CGameObject::Init(); // chỗ này bị lỗi nếu để Init là thuần ảo. Ta phải để Init là ảo thôi để có thể gọi được ngay trong cha của nó

}

CGameObject::~CGameObject()
{
	/*for (auto a : animations)
		delete a.second;*/
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
		physiscBody->PhysicsUpdate(collisionBox, &otherCollisionBoxs); // boxes :D xin loi canh sat :(
	}
}

//void CGameObject::Update(DWORD dt, CCamera* cam)
//{
//	DebugOut(L"[INFO] Game Object Updating.. \n");
//
//}

void CGameObject::LateUpdate()
{
}

void CGameObject::Render(CCamera* cam)
{
	//DebugOut(L"[INFO] Render Game Object \n");
	//OutputDebugString(ToLPCWSTR("[INFO] Current State:" + currentState + "\n"));
	//collisionBoxs->at(0)->RenderBoundingBox();

	
	bool curState = animations.find(currentState) != animations.end();
	if (curState == NULL || animations.empty())
		return;
	D3DXVECTOR2 posInCam = cam->Transform(transform.position + relativePositionOnScreen);
	//D3DXVECTOR2 posInCam = cam->Transform(transform.position);
	//DebugOut(ToLPCWSTR("Position: " + std::to_string(transform.translatePos.x) + "\n"));
	animations.at(currentState)->Render(posInCam);
}

void CGameObject::AnimationUpdate()
{
	bool curState = animations.find(currentState) != animations.end();
	if (animations.empty() || curState == false) return;
	//animations.at(currentState)->SetPosition(transform.position + relativePositionOnScreen); // *****
	animations.at(currentState)->Update();
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
