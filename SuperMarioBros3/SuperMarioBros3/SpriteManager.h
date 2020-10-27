#pragma once

#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>
#include "Sprite.h"

class CSpriteManager;
typedef CSpriteManager* LPSpriteManager;
class CSpriteManager
{
	static LPSpriteManager instance;

	std::unordered_map<std::string, LPSprite> sprites;

public:
	CSpriteManager();
	void Init();
	void Clear();
	void Add(std::string id, RECT rect, LPDIRECT3DTEXTURE9 tex, int xPivot, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255,0,255));
	bool LoadSprite(std::string texName, std::string filePath);
	LPSprite Get(std::string id);
	static LPSpriteManager GetInstance();
	~CSpriteManager();
};
#endif
