#include "GameObject.h"

// Tạm thời
#include "Game.h"
#include "TextureManager.h"
#include <string>
using namespace std;
CGameObject::CGameObject()
{
	this->currentState = "";
}

CGameObject::~CGameObject()
{
	
}

void CGameObject::Render()
{
	// Tạm thời thui, lúc sau sẽ gọi bên Update của Scene
	AnimationUpdate();
	DebugOut(L"[INFO] Render Game Object \n");
	OutputDebugString(ToLPCWSTR("[INFO] Current State:" + currentState + "\n"));
	
	bool curState = animations.find(currentState) != animations.end();
	if (curState == NULL || animations.empty())
	{
			DebugOut(L"Dont have Animation \n");
			return;
	}
	// position???
	DebugOut(ToLPCWSTR("Position: " + std::to_string(position.x) + "\n"));
	animations.at(currentState)->Render(position); 

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

void CGameObject::SetState(string state)
{
	currentState = state;
}
