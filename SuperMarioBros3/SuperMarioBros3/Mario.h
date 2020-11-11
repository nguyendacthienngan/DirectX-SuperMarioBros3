#pragma once

#ifndef MARIO_H
#define MARIO_H

#include "GameObject.h"
#include "MarioStateSet.h"
#include "KeyboardManager.h"
#include "IState.h"
#include "Holdable.h"

class CMario : public CGameObject, public IState
{
	// Mario Base : Xử lý những thứ cơ bản nhất trước của Mario
	// Đi, chạy, nhảy, hụp, thắng lại
protected:
	MarioStates marioStateTag;
	D3DXVECTOR2 targetVelocity, previousVelocity; 
	MarioStateSet currentPhysicsState, previousPhysicsState; // state vật lý, còn currentState ở GameObject là state animation
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
	CHoldable* objectHolding;
	bool isHold, isKick;
	bool isDamaged, isSmokeEffectAnimation, isChangeLevel;
	DWORD timeStartSmokeEffect;
	DWORD timeStartChangeLevel;
	DWORD timeStartDamaged;
	DWORD timeStartChangeAlpha;
	int countSmokeEffectActivate;
	int countChangeAlpha;

public:
	CMario();
	void SetDamageFlag(bool isDamaged);
	void SetChangeSmokeEffectFlag(bool isChangeSmokeEffect);
	void SetChangeLevelFlag(bool flag);

	void SetCountChangeAlpha(int count);
	void SetCountSmokeEffectActivate(int count);

	void SetTimeStartDamaged(DWORD t);
	void SetTimeStartSmokeEffect(DWORD t);
	void SetTimeStartChangeLevel(DWORD t);
	void SetTimeStartChangeAlpha(DWORD t);

	bool GetDamageFlag();
	bool GetChangeSmokeEffectFlag();
	bool GetChangeLevelFlag();

	bool GetCountChangeAlpha();
	bool GetCountSmokeEffectActivate();

	DWORD GetTimeStartDamaged();
	DWORD GetTimeStartSmokeEffect();
	DWORD GetTimeStartChangeLevel();
	DWORD GetTimeStartChangeAlpha();

	void Init() override;
	virtual void InitProperties();
	void LoadAnimation();
	void EndAnimation() override;

	void Update(DWORD dt, CCamera* cam) override;
	void Render(CCamera* cam, int alpha = 255) override;

	void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions); // Xử lý riêng sau khi phát hiện va chạm
	void OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions); // Tạm thời chưa làm
	void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox) override;

	void CrouchProcess(CKeyboardManager* keyboard);
	void SkidProcess(D3DXVECTOR2 velocity);
	void HoldProcess();
	void JumpProcess(float jumpForce, bool bounceAfterJumpOnEnemy);
	void KickProcess(bool isKick);
	void DamageProcess();

	void StopBounce(bool stopBounce);
	bool StopBounce();

	void KeyState();
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);

	void OnDamaged();
	void HoldObject(CHoldable* holdableObj);
	void SetMarioStateTag(MarioStates tag);
	MarioStates GettMarioStateTag();

	bool CanRun();

	virtual void Access()		override;
	virtual void Process()		override;
	virtual void Exit()			override;

	~CMario();
};
#endif
