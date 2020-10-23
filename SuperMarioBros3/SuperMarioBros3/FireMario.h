#pragma once
#include "Mario.h"
class CFireMario : public CMario
{
private:
	bool isThrowingFireBall;
public:
	CFireMario();
	void Init() override;
	void LoadAnimation() override;
	void Render(CCamera* cam) override;

	void OnKeyDown(int KeyCode) override;
};

