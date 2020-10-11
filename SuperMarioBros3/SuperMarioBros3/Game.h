#pragma once

#ifndef GAME_H
#define GAME_H

#include <d3dx9.h>
#include <d3d9.h>
#include <dinput.h>

#include "KeyEventHandler.h"
#include "GameConst.h"

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
	
	LPDIRECTINPUT8       dInput;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 dInputDevice;		// Device có thể là là bất kỳ thiết bị nào. Nhưng trg game của mình thì chỉ có keyboard nên đây là Keyboard device 

	LPKeyEventHandler keyEventHandler;

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	/*Draw*/
	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL; // Sprite helper libary


public:
	static CGame* GetInstance();
	~CGame();
	void InitDirectX(HWND hWnd, int scrWidth, int scrHeight);
	void InitKeyboard (LPKeyEventHandler handler);
	void Draw(float x, float y, int xCenter, int yCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha, D3DXVECTOR2 scale, float rotation);
	void Draw(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha, D3DXVECTOR2 scale, float rotation);
	void Init();
	void Run(); // while-loop game
	void End();
	
	void Render();
	void Update();
	// Chỗ này cần xem lại cách tổ chức
	void ProcessKeyboard(); // Khi Game đang chạy ta sẽ gọi Process keyboard liên tục để kiểm tra
	bool CheckESCKey();

	int GetScreenWidth() { return screenWidth; }
	int GetScreenHeight() { return screenHeight; }

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }
	LPKeyEventHandler GetKeyEventHandler() { return this->keyEventHandler; }

	// Chỗ này cần xem lại cách tổ chức
	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);

	static float GetTimeScale() { return timeScale; }
	static void SetTimeScale(float time) { timeScale = time; }
};

#endif