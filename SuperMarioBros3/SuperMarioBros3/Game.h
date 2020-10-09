#pragma once

#ifndef GAME_H
#define GAME_H

#include <d3dx9.h>
#include <d3d9.h>

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
	void Init();
	void InitDirectX(HWND hWnd, int scrWidth, int scrHeight);
	
	LPDIRECT3DTEXTURE9 LoadTexture();
	void Run(); // while-loop game
	
	void Draw(float x, float y, int xCenter, int yCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha);
	void Render();
	//void Update();

	int GetScreenWidth() { return screenWidth; }
	int GetScreenHeight() { return screenHeight; }


	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	static float GetTimeScale() { return timeScale; }
	static void SetTimeScale(float time) { timeScale = time; }
};

#endif