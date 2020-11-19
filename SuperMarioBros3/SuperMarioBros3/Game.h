#pragma once

#ifndef GAME_H
#define GAME_H

#include <d3dx9.h>
#include <d3d9.h>
#include <dinput.h>
#include <unordered_map>

#include "KeyEventHandler.h"
#include "GameConst.h"

#include "KeyboardManager.h"
class CGame
{
private:
	int screenWidth;
	int screenHeight;

	int fps;

	static CGame* instance;
	static float timeScale;
	static DWORD deltaTime;

	HWND hWnd; // Window Handle

	LPDIRECT3D9 d3d = NULL; // Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL; // Direct3D device handle
	
	/*Draw*/
	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL; // Sprite helper libar

	// Sau khi đọc file root.xml, ta sẽ lưu các thông tin file đó dưới dạng 1 map chứa 1 map
	// Mỗi category (Textures, Sprites, Animations, Scenes, Config) sẽ có các bucket
	// các bucket sẽ giữ id và source (filePath)
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> gameSource;

public:
	static CGame* GetInstance();
	~CGame();
	void InitDirectX(HWND hWnd, int scrWidth, int scrHeight, int fps);
	void Draw(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255,0,255));
	void Draw(D3DXVECTOR2 position, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha);

	void DrawFlipX(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255));
	void DrawFlipY(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255));
	
	void Init();
	void Run(); // while-loop game
	void End();
	
	void Render();
	void Update();

	int GetScreenWidth() { return screenWidth; }
	int GetScreenHeight() { return screenHeight; }

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	static float GetTimeScale() { return timeScale; }
	static void SetTimeScale(float time) { timeScale = time; }

	float GetFixedDeltaTime() { return 20; }
	DWORD GetDeltaTime() { return deltaTime;  }

	bool ImportGameSource();
	std::string GetFilePathByCategory(std::string category, std::string id);
};

#endif