#pragma once
#include "Gift.h"
class CMushroomEffect : public CGift
{
private:
	bool isAppearing;
	float marioFacing;
public:
	CMushroomEffect();
	void LoadAnimation();
	virtual void Update(DWORD dt, CCamera* cam);
	void Render(CCamera* cam, int alpha = 255) override;
	void StartEffect(float MarioFacing);
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
};

