#include "Game.h"
#include "GameConst.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "GameKeyEventHandler.h"

#include "Scene1.h"
CGame* CGame::instance = NULL;
float CGame::deltaTime = 0.0f;
float CGame::timeScale = 1.0f;

CGame* CGame::GetInstance()
{
    if (instance == NULL) instance = new CGame();
    return instance;
}

CGame::~CGame() // đúng hay k. Hỏi thầy
{
	//DebugOut(L"[INFO] This game is about to end");
	//if (spriteHandler != NULL) spriteHandler->Release();
	//if (backBuffer != NULL) backBuffer->Release();
	//if (d3ddv != NULL) d3ddv->Release();
	//if (d3d != NULL) d3d->Release();
	////Kill_Keyboard();
	//if (dinput != NULL) dinput->Release();
}

void CGame::Init()
{
	CTextureManager::GetInstance()->Init();
	CSpriteManager::GetInstance()->Init();
	CAnimationManager::GetInstance()->Init();

	keyEventHandler = new GameKeyEventHandler();
	InitKeyboard(keyEventHandler); // ***

	CScene1* scene1 = new CScene1();
	CSceneManager::GetInstance()->Load(scene1);
	
	fps = 60;
	
}

void CGame::InitDirectX(HWND hWnd, int scrWidth, int scrHeight)
{
	this->hWnd = hWnd;
	this->screenWidth = scrWidth;
	this->screenHeight = scrHeight;
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

void CGame::InitKeyboard(LPKeyEventHandler handler)
{
	DebugOut(L"[ERROR] HWND: \n");
	HRESULT
		hr = DirectInput8Create
		(
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&dInput, NULL
		);
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DirectInput8 Create failed!\n");
		return;
	}

	hr = dInput->CreateDevice(GUID_SysKeyboard, &dInputDevice, NULL);

	// TO-DO: put in exception handling
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] CreateDevice failed!\n");
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.

	hr = dInputDevice->SetDataFormat(&c_dfDIKeyboard);

	// Chỉ định game ta không chiếm trọn bàn phím mà ta chia sẻ với các chương trình khác
	hr = dInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	hr = dInputDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = dInputDevice->Acquire();
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DINPUT8::Acquire failed!\n");
		return;
	}

	this->keyEventHandler = handler; // Truyền đối tượng xử lý từng phím (handler) vào bên trong keyHandler

	DebugOut(L"[INFO] Keyboard has been initialized successfully\n");
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
			Update();
			// Process key
			ProcessKeyboard(); 
			if (CheckESCKey() == true)
				continue;
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

void CGame::End()
{
	DebugOut(L"[INFO] This game is about to end");
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
	if (dInput != NULL)
	{
		dInput->Release();
		dInputDevice->Unacquire();
		dInputDevice->Release();
	}
	DebugOut(L"[INFO] Bye bye \n");

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

void CGame::Update()
{
	LPSceneManager sceneManger = CSceneManager::GetInstance();
	LPScene activeScene = sceneManger->GetActiveScene();
	// Update Scene. Trong Scene sẽ Update các GameObject. Trong GameObject sẽ update các animation. Các animation sẽ update các animation frame / sprite ?
	if (activeScene != NULL)
		activeScene->Update(deltaTime);
}

void CGame::ProcessKeyboard() //Cần chỉnh sửa nhiều
{
	HRESULT hr;
	// Collect all key states first:Đọc trạng thái của toàn bộ bàn phím. Mỗi một byte ứng với một phím
	hr = dInputDevice->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = dInputDevice->Acquire();
			if (h == DI_OK)
			{
				DebugOut(L"[INFO] Keyboard re-acquired!\n");
			}
			else return;
		}
		else
		{
			DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	// TODO: Ta gọi 1 hàm ảo của 1 đối tượng truyền vô để xử lý
	//-------------------------------------- CHƯA LÀM -------------------------
	if (keyEventHandler == nullptr) return;
	//keyEventHandler->KeyState((BYTE*)&keyStates);



	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = dInputDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0) // Up xuống Down
			keyEventHandler->OnKeyDown(KeyCode); // Do object truyền vào
		else // Down lên Up
			keyEventHandler->OnKeyUp(KeyCode);
	}
	
}

bool CGame::CheckESCKey()
{

	if (this->GetKeyDown(DIK_ESCAPE))
	{
		DebugOutTitle(L"Nhan ESC");
		DebugOut(L"Nhan ESC \n");
		CGame::GetInstance()->End();
		PostQuitMessage(0);
		return true;
	}
	return false;
}

// Hiện tại chưa chạy được
bool CGame::GetKeyDown(int keyCode)
{
	return (keyStates[keyCode] & 0x80) > 0; // Lấy ra nút được ấn
}

bool CGame::GetKeyUp(int keyCode)
{
	return (keyStates[keyCode] & 0x80) <= 0;
}
