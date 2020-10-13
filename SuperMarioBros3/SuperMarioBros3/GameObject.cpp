#include "GameObject.h"

// Tạm thời
#include "Game.h"
#include "TextureManager.h"
#include <string>
using namespace std;
CGameObject::CGameObject()
{
	//CGameObject::Init(); // chỗ này bị lỗi nếu để Init là thuần ảo. Ta phải để Init là ảo thôi để có thể gọi được ngay trong cha của nó
	this->currentState = "";
	//this->SetScale(D3DXVECTOR2(3, 3)); // bị lỗi vì Animation chưa có để set scale : Có cách nào để anim với object đồng bộ hoặc bỏ bớt transform trg anim k?
	
}

CGameObject::~CGameObject()
{
	for (auto a : animations)
		delete a.second;
}


void CGameObject::Init()
{
	
}

void CGameObject::Update(DWORD dt, std::vector<LPGameObject>* coObjects)
{
	DebugOut(L"[INFO] Game Object Updating.. \n");

}

void CGameObject::Render()
{
	DebugOut(L"[INFO] Render Game Object \n");
	OutputDebugString(ToLPCWSTR("[INFO] Current State:" + currentState + "\n"));
	
	bool curState = animations.find(currentState) != animations.end();
	if (curState == NULL || animations.empty())
	{
		if(animations.empty())
			DebugOut(L"Dont have Animation \n");
		if(curState == NULL)
			DebugOut(L"Cannot find curState \n");
			return;
	}
	DebugOut(ToLPCWSTR("Position: " + std::to_string(transform.translatePos.x) + "\n"));
	animations.at(currentState)->Render(transform.translatePos);

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

void CGameObject::SetState(string state)
{
	currentState = state;
}
