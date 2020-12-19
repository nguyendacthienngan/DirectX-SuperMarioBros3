#pragma once
#include "Sprite.h"
#include <map>
class CCardGift
{
private:
	std::map<std::string, LPSprite> cardSprites;
	D3DXVECTOR2 pos;
	std::string state;
public:
	CCardGift();
	CCardGift(D3DXVECTOR2 cardPos);
	void LoadSprite();
	void Update();
	void Render();
	void SetState(std::string state);
	void SetPosition(D3DXVECTOR2 pos);
};