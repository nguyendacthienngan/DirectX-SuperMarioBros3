#pragma once

#ifndef MARIO_H
#define MARIO_H

#include "GameObject.h"
#include "MarioState.h"

class CMario : public CGameObject
{
	// Mario Base : Xử lý những thứ cơ bản nhất trước
private:
	MarioStateSet state;
	bool isOnGround;
public:
	CMario();
	void Init();
	void LoadAnimation();
	void Update(DWORD dt, CCamera* cam);
	
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
	void OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
	
	void KeyState();
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	~CMario();
};
#endif
