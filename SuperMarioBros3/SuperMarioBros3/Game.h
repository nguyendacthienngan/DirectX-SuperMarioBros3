#pragma once

#ifndef GAME_H
#define GAME_H

#include <d3dx9.h>
#include <d3d9.h>
#include <dinput.h>

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
	static float deltaTime;

	HWND hWnd; // Window Handle

	LPDIRECT3D9 d3d = NULL; // Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL; // Direct3D device handle
	
	/*Draw*/
	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL; // Sprite helper libary


public:
	static CGame* GetInstance();
	~CGame();
	void InitDirectX(HWND hWnd, int scrWidth, int scrHeight);
	//void Draw(float x, float y, int xCenter, int yCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha, D3DXVECTOR2 scale, float rotation);
	void Draw(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha);

	//void Draw(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha, D3DXVECTOR2 scale, float rotation);
	
	void DrawFlipX(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha);
	void DrawFlipY(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha);
	void Init();
	void Run(); // while-loop game
	void End();
	
	void Render();
	void Update();

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	int GetScreenWidth() { return screenWidth; }
	int GetScreenHeight() { return screenHeight; }

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	static float GetTimeScale() { return timeScale; }
	static void SetTimeScale(float time) { timeScale = time; }

	float GetFixedDeltaTime() { return 0.02;  }
};

#endif