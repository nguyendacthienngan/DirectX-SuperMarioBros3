#pragma once
#include "Mario.h"
#include "FireBall.h"
#include "Scene.h"

class CScene;
class CFireMario : public CMario
{
private:
	std::vector<CGameObject*> listFireBalls;
	CGameObject* currentFireBall;
public:
	CFireMario();
	void Init() override;
	void LoadAnimation() override;
	void Render(CCamera* cam) override;
	void Update(DWORD dt, CCamera* cam) override;
	void EndAnimation();
	void AddMiscToScene(CScene* scene) override;
	void OnKeyDown(int KeyCode) override;
};

