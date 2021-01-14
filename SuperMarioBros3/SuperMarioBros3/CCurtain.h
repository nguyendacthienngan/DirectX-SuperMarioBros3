#pragma once
#include "Sprite.h"
class CCurtain
{
private:
	CSprite* curtainSprite;
	D3DXVECTOR2 pos;
public: 
	CCurtain();
	void Update();
	void Render();
};

