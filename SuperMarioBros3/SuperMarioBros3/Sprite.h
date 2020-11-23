#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

class CSprite;
typedef CSprite* LPSprite;
class CSprite
{
	std::string id;				// Sprite ID in the sprite database
	RECT rect;
	int width;
	int height;
	D3DXVECTOR2 pointCenter;
	LPDIRECT3DTEXTURE9 texture;
	D3DXCOLOR transcolor;
	
public:
	CSprite(std::string id, int xPivot, RECT rect = RECT(), LPDIRECT3DTEXTURE9 tex = NULL, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255)); // texture là bên Texture manager giữ
	void Draw(D3DXVECTOR2 position, D3DXVECTOR2 scale, float rotation, D3DXCOLOR transcolor = D3DXCOLOR(254, 255, 255, 255));

	RECT GetRect();
	void SetRect(RECT rect);
	int GetWidth();
	void SetWidth(int w);
	int GetHeight();
	void SetHeight(int h);

	D3DXVECTOR2 GetPointCenter();
	void SetPointCenter(D3DXVECTOR2 pointCenter);

	D3DXCOLOR GetTranscolor();
	void SetTranscolor(D3DXCOLOR transcolor);
	~CSprite();
};

#endif