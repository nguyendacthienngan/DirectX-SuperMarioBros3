#include "Sprite.h"
#include "Game.h"

#include "Ultis.h"

CSprite::CSprite(std::string id, RECT rect, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->rect = rect;
	this->texture = tex;
	this->width = rect.right - rect.left;
	this->height = rect.top - rect.bottom;
	this->pointCenter = D3DXVECTOR2(width/2, height/2);
}

void CSprite::Draw(D3DXVECTOR2 position, D3DXVECTOR2 scale, float rotation, int alpha)
{
	DebugOut(L"[INFO] Draw Sprite \n");
	CGame* game = CGame::GetInstance();
	game->Draw(position, pointCenter, texture, rect, alpha, scale, rotation);
}

int CSprite::GetWidth()
{
	return width;
}

void CSprite::SetWidth(int w)
{
	width = w;
}

int CSprite::GetHeight()
{
	return height;
}

void CSprite::SetHeight(int h)
{
	height = h;
}
