#pragma once
#include "Sprite.h"
class CArrowItemIcon
{
private:
	D3DXVECTOR2 pos;
	D3DXVECTOR2 size;

	LPSprite blackArrowSprite;
	LPSprite whiteArrowSprite;
	LPSprite currentSprite;
	bool isCharged;
public:
	CArrowItemIcon();
	void LoadSprite();
	void SetCharged(bool isCharged);
	bool IsCharged();
	void Update();
	void Render();

	D3DXVECTOR2 GetPosition();
	void SetPosition(D3DXVECTOR2 pos);

	D3DXVECTOR2 GetSize();
	void SetWidth(float w);
	void SetHeight(float h);
};

