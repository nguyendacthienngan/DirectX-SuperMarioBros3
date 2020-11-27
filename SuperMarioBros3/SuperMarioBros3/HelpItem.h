#pragma once
#include "GameObject.h"
class CHelpItem : public CGameObject
{
private:
	DWORD timeStartChangeAlpha;
	DWORD timeToChangeAlpha;
	DWORD timeStopChangeAlpha;
	int countChangeAlpha;
public:
	CHelpItem();
	void LoadAnimation() override;
	virtual void Render(CCamera* cam, int alpha = 255);

};

