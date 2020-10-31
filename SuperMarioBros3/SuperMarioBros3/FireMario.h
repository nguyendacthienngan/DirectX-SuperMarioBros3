#pragma once
#include "Mario.h"
#include "FireBall.h"
#include "Scene.h"

class CScene;
class CFireMario : public CMario
{
private:
	//bool isJumpAttack;
	int countFireBall;
	DWORD timeToNextAttack, lastAttackTime;
public:
	CFireMario();
	void Init() override;
	void Clear() override;
	void LoadAnimation() override;
	void Render(CCamera* cam) override;
	void Update(DWORD dt, CCamera* cam) override;
	void EndAnimation();
	void OnKeyDown(int KeyCode) override;
	
	~CFireMario();
};

