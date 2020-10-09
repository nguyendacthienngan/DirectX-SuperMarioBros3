#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Animation.h"


class CGameObject;
typedef CGameObject* LPGameObject;
class CGameObject
{
private:
	int id;
	DWORD dt;

	D3DXVECTOR2 position;
	D3DXVECTOR2 velocity;
	D3DXVECTOR2 distance; // dx = vx*dt, dy = vy*dt, độ dời sau một khoảng thời gian
	D3DXVECTOR2 normal; // vector pháp tuyến? nx, ny

	//STATE?
	std::string currentState;

	// Mỗi gameobject sẽ lưu animation của riêng nó
	std::unordered_map<std::string, LPAnimation> animations;
public:
	CGameObject();
	~CGameObject();

	//virtual void Update(DWORD dt, std::vector<LPGameObject>* coObjects = NULL); // Truyền một vector GameObject để xét va chạm với GameObject hiện tại
	virtual void Render();
	virtual void AnimationUpdate();

	void AddAnimation(std::string stateName, LPAnimation animation);

	void SetPosition(D3DXVECTOR2 p) { this->position = p; }
	void SetSpeed(D3DXVECTOR2 v) { this->velocity = v; }
	D3DXVECTOR2 GetPosition() { return position; }
	D3DXVECTOR2 GetSpeed() { return velocity; }
	void SetState(std::string state);

};

#endif