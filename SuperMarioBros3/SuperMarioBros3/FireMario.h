#pragma once
#include "Mario.h"
#include "Scene.h"
#include "ObjectPool.h"

class CScene;
class CFireMario : public CMario
{
private:
	int countFireBall;
	DWORD timeToNextAttack, lastAttackTime;
	CObjectPool fireBalls;
public:
	CFireMario();
	void Init() override;
	void Clear() override;
	void LoadAnimation() override;
	void Render(CCamera* cam, int alpha = 255) override;
	void Update(DWORD dt, CCamera* cam) override;
	void EndAnimation();
	void OnKeyDown(int KeyCode) override;
	
	void AddObjectToScene(LPScene scene);
	~CFireMario();
};

