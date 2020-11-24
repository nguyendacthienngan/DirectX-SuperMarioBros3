
#include <string>

#include "TextureManager.h"
#include "Game.h"
#include "GraphicConst.h"
#include "Const.h"

using namespace std;

LPTextureManager CTextureManager::instance = NULL;


LPTextureManager CTextureManager::GetInstance()
{
	if (instance == NULL) instance = new CTextureManager();
	return instance;
}

void CTextureManager::Init()
{
	auto root = CGame::GetInstance();
	LoadTexture(TEXTURE_MARIO, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_MARIO));
	LoadTexture(TEXTURE_BBOX, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_BBOX));
	LoadTexture(TEXTURE_MISC, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_MISC));
	LoadTexture(TEXTURE_FIRE_BALL, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_FIRE_BALL));
	LoadTexture(TEXTURE_ENEMY, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_ENEMY));
	LoadTexture(TEXTURE_EFFECT, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_EFFECT));
	LoadTexture(TEXTURE_UI, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_UI));
	LoadTexture(TEXTURE_BLACK, root->GetFilePathByCategory(CATEGORY_TEXTURE, TEXTURE_BLACK));
}

void CTextureManager::Clear()
{
	for (auto t : textures)
	{
		LPDIRECT3DTEXTURE9 tex = t.second;
		if (tex != NULL) tex->Release();
	}
	textures.clear();
}

void CTextureManager::LoadTexture(string  texName, string texPath)
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

void CTextureManager::Add(string id, std::string filePath, D3DCOLOR transparentColor)
{
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(ToLPCWSTR(filePath), &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DTEXTURE9 texture;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		ToLPCWSTR(filePath),							// Path to the image to load
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

	//textures.at(id) = texture;
	textures.insert(make_pair(id, texture));

	OutputDebugStringW(ToLPCWSTR("[INFO] Texture loaded Ok:  " + filePath + "\n"));
	OutputDebugStringW(ToLPCWSTR("[INFO] Texture ID: " + id + "\n"));
}

CTextureManager::~CTextureManager()
{
	
}
