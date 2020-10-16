
#include <string>

#include "TextureManager.h"
#include "Game.h"
#include "GraphicConst.h"

using namespace std;

LPTextureManager CTextureManager::instance = NULL;


LPTextureManager CTextureManager::GetInstance()
{
	if (instance == NULL) instance = new CTextureManager();
	return instance;
}

void CTextureManager::Init()
{
	LoadTexture(MARIO_TEXTURE, ToLPCWSTR(SPRITE_PATH + MARIO_IMG));
}

void CTextureManager::LoadTexture(string texName, LPCWSTR texPath)
{
	Add(texName, texPath, D3DCOLOR_XRGB(255, 255, 255));
}

LPDIRECT3DTEXTURE9 CTextureManager::GetTexture(std::string id)
{
	if (textures.find(id) == textures.end())
		return nullptr;
	else 
		return textures.at(id);
}

void CTextureManager::Add(string id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DTEXTURE9 texture;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		filePath,							// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	textures[id] = texture;

	DebugOut(L"[INFO] Texture loaded Ok: %s\n", filePath);
	OutputDebugStringW(ToLPCWSTR("[INFO] Texture ID: " + id + "\n"));
}

CTextureManager::~CTextureManager()
{
	/*for (auto t : textures)
		delete t.second;*/
}
