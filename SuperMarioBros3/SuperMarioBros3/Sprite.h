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
public:
	CSprite(std::string id, RECT rect = RECT(), LPDIRECT3DTEXTURE9 tex = NULL); // texture là bên Texture manager giữ
	void Draw(D3DXVECTOR2 position, D3DXVECTOR2 scale, float rotation, int alpha = 255);

	int GetWidth();
	void SetWidth(int w);
	int GetHeight();
	void SetHeight(int h);
};

#endif