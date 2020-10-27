#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>
#include "Ultis.h"

class CTextureManager;
typedef CTextureManager* LPTextureManager;
class CTextureManager
{
private:
	static LPTextureManager instance;
	std::unordered_map<std::string, LPDIRECT3DTEXTURE9> textures;

public:
	static LPTextureManager GetInstance();
	void Init();
	void Clear();
	void LoadTexture(std::string texName, std::string texPath);
	LPDIRECT3DTEXTURE9 GetTexture(std::string id);
	void Add(std::string id, std::string filePath, D3DCOLOR transparentColor);
	~CTextureManager();
};

#endif