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

void CGameObject::Update(DWORD dt, CCamera* cam)
{
	//DebugOut(L"[INFO] Game Object Updating.. \n");

}

void CGameObject::Render(CCamera* cam)
{
	//DebugOut(L"[INFO] Render Game Object \n");
	//OutputDebugString(ToLPCWSTR("[INFO] Current State:" + currentState + "\n"));
	
	bool curState = animations.find(currentState) != animations.end();
	if (curState == NULL || animations.empty())
	{
		/*if (animations.empty())
		{
			DebugOut(L"Dont have Animation \n");

		}
		if (curState == NULL)
		{
			DebugOut(L"Cannot find curState \n");
		}*/
		return;

	}
	D3DXVECTOR2 posInCam = cam->Transform(transform.position);
	//DebugOut(ToLPCWSTR("Position: " + std::to_string(transform.translatePos.x) + "\n"));
	animations.at(currentState)->Render(posInCam);

}

void CGameObject::AnimationUpdate()
{
	bool curState = animations.find(currentState) != animations.end();
	if (animations.empty() || curState == false) return;
	animations.at(currentState)->Update();
}

void CGameObject::AddAnimation(std::string stateName, LPAnimation animation)
{
	animations.insert(make_pair(stateName, animation));
}

bool CGameObject::IsEnabled()
{
	return isEnabled;
}

void CGameObject::RenderBoundingBox()
{
	// TO-DO
	/*D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextureManager::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);*/
}

void CGameObject::SetState(string state)
{
	currentState = state;
}
