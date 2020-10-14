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

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCollisionEvent CGameObject::SweptAABBEx(LPGameObject coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	svx = coO->GetSpeed().x;
	svy = coO->GetSpeed().y;

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->distance.x - sdx;
	float rdy = this->distance.y - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

void CGameObject::CalcPotentialCollisions(
	std::vector<LPGameObject>* coObjects, 
	std::vector<LPCollisionEvent>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCollisionEvent e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCollisionEvent>& coEvents,
	vector<LPCollisionEvent>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCollisionEvent c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void CGameObject::SetState(string state)
{
	currentState = state;
}
