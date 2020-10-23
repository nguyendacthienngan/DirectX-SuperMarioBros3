#pragma once

#ifndef MARIO_H
#define MARIO_H

#include "GameObject.h"
#include "MarioStateSet.h"
#include "KeyboardManager.h"

class CMario : public CGameObject
{
	// Mario Base : Xử lý những thứ cơ bản nhất trước của Mario
	// Đi, chạy, nhảy, hụp, thắng lại
private:
	D3DXVECTOR2 targetVelocity, previousVelocity, previousTargetVelocity; // đến một mức velocity thì thay đổi
	MarioStateSet currentPhysicsState, previousPhysicsState; // state vật lý, còn currentState ở GameObject là state animation
	bool isOnGround;
	bool isHighSpeed; // horizontal
	bool canLowJumpContinous;
	bool isHighJump, canHighJump; 
	bool isSkid;
	D3DXVECTOR2 previousNormal;
public:
	CMario();
	void Init() override;
	void LoadAnimation();
	void Update(DWORD dt, CCamera* cam) override;
	void Render(CCamera* cam);
	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions); // Xử lý riêng sau khi phát hiện va chạm
	void OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions); // Tạm thời chưa làm
	
	void CrouchProcess(CKeyboardManager* keyboard);
	void SkidProcess(D3DXVECTOR2 velocity);

	void KeyState();
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	~CMario();
};
#endif
