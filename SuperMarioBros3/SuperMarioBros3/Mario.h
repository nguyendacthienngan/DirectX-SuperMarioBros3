#pragma once

#ifndef MARIO_H
#define MARIO_H

#include "GameObject.h"
#include "MarioStateSet.h"
#include "KeyboardManager.h"
#include "IState.h"

class CMario : public CGameObject, public IState
{
	// Mario Base : Xử lý những thứ cơ bản nhất trước của Mario
	// Đi, chạy, nhảy, hụp, thắng lại
protected:
	MarioStates marioStateTag;
	D3DXVECTOR2 targetVelocity, previousVelocity; 
	MarioStateSet currentPhysicsState; // state vật lý, còn currentState ở GameObject là state animation
	bool isOnGround;
	bool isHighSpeed; // horizontal
	bool canLowJumpContinous;
	bool isHighJump, canHighJump, isJump; 
	int isSkid;
	bool canCrouch;
	D3DXVECTOR2 previousNormal;
	bool canAttack, isAttack, canAttackContinious;
	bool isJumpAttack;
	int feverState; // - 1, 0, 1, 2 : -1 là disable đối với Raccoon vì Raccoon bay kiểu khác
	DWORD feverTime; // Thời gian PMeter giữ giá trị max
	DWORD lastFeverTime;
	DWORD beginAttackTime;
	float pMeterCounting;
	float beforeJumpPosition;
	bool canFly;
	bool isFly;
	bool bounceAfterJumpOnEnemy, stopBounce;
public:
	CMario();
	void Init() override;
	virtual void InitProperties();
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

	void OnDamaged();

	void SetMarioStateTag(MarioStates tag);
	MarioStates GettMarioStateTag();

	virtual void Access()		override;
	virtual void Process()		override;
	virtual void Exit()			override;

	~CMario();
};
#endif
