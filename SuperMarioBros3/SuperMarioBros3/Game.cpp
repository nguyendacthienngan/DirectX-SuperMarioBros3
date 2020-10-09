#include "Game.h"
#include "GameConst.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"
#include "Ultis.h"

#include "Scene1.h"
CGame* CGame::instance = NULL;
float CGame::deltaTime = 0.0f;
float CGame::timeScale = 1.0f;

CGame* CGame::GetInstance()
{
    if (instance == NULL) instance = new CGame();
    return instance;
}

CGame::~CGame()
{
	spriteHandler->Release();
	backBuffer->Release();
	d3ddv->Release();
	d3d->Release();
}

void CGame::Init()
{
	CTextureManager::GetInstance()->Init();
	CSpriteManager::GetInstance()->Init();
	CAnimationManager::GetInstance()->Init();
	//DebugOut(CAnimationManager::GetInstance()->Get("ani-small-mario-run") ? L"Load Animation successfully" : L"DM");
	
	CScene1* scene1 = new CScene1();
	CSceneManager::GetInstance()->Load(scene1);
	
	fps = 60;
	
}

void CGame::InitDirectX(HWND hWnd, int scrWidth, int scrHeight)
{
	DebugOut(L"[INFO] Begin Init DirectX \n");
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = screenWidth = scrWidth;
	d3dpp.BackBufferHeight = screenHeight = scrHeight;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.Windowed = true;
	d3dpp.hDeviceWindow = hWnd;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	D3DXCreateSprite(d3ddv, &spriteHandler);

	if (!spriteHandler)
	{
		MessageBox(hWnd, L"Creating sprite handler failed!", L"Error", MB_OK | MB_ICONERROR);
		return;
	}
	DebugOut(L"[INFO] Init DirectX Done \n");
}

void CGame::Run()
{
	MSG msg;
	bool done = false;

	float prevTime, currentTime = GetTickCount();
	float delta = 0;
	float tickPerFrame = 1000.0f / fps;

	// Game Loop
	while (!done)
	{
		prevTime = currentTime;
		currentTime = GetTickCount();
		delta += (currentTime - prevTime);
		deltaTime = delta;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Call update
			//Update();
			//GetService<InputHandler>()->ProcessKeyboard();

			// Render in limited fps
			if (delta >= tickPerFrame)
			{
				Render();
				if (delta > tickPerFrame) delta = 0.0f;
			}
			else
			{
				Sleep(tickPerFrame - delta);
				delta = tickPerFrame;
			}
		}
	}
}

void CGame::Draw(float x, float y, int xCenter, int yCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha)
{
	D3DXVECTOR3 p(x, y, 0);
	D3DXVECTOR3 pCenter(xCenter, yCenter, 0);
	// Vẽ bằng góc trái trên
	//spriteHandler->Draw(texture, &rect, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));

	// Vẽ bằng tâm
	spriteHandler->Draw(texture, &rect, &pCenter, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));

}

void CGame::Render()
{
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	if (d3ddv->BeginScene())
	{
		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
		if (activeScene != nullptr)
			activeScene->Render();

		spriteHandler->End();

		d3ddv->EndScene();
	}

	d3ddv->Present(NULL, NULL, NULL, NULL);

}
