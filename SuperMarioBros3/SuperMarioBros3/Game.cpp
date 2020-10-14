#include "Game.h"
#include "GameConst.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "GameKeyEventHandler.h"

#include "Scene1.h"

#include <string>
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
	fps = 60;

	CTextureManager::GetInstance()->Init();
	CSpriteManager::GetInstance()->Init();
	CAnimationManager::GetInstance()->Init();

	CGameKeyEventHandler *keyEventHandler = new CGameKeyEventHandler();
	auto keyboardManager = CKeyboardManager::GetInstance();
	keyboardManager->SetHWND(hWnd);
	keyboardManager->InitKeyboard(keyEventHandler);
	
	CScene1* scene1 = new CScene1();
	CSceneManager::GetInstance()->Load(scene1);
	
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
		prevTime = currentTime; // framestart?
		currentTime = GetTickCount(); // now
		delta += (currentTime - prevTime);
		deltaTime = delta; 
		// Do trong game không có thời gian thực sự, để tạo cảm giác thời gian trôi qua thì ta chỉ có bộ đếm tick từ lúc start game để tạo cảm giác chân thực

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Update and Render in limited fps
			if (delta >= tickPerFrame) // chuyển frame mới
			{
				// Call update
				Update();
				// Process key
				auto keyboardManger = CKeyboardManager::GetInstance();
				keyboardManger->ProcessKeyboard();
				if (keyboardManger->CheckESCKey() == true)
					continue;
				Render();

				if (delta > tickPerFrame) delta = 0.0f;
			}
			else // chưa tới tickperframe nên cho ngủ vì xong việc cho 1 frame ròi
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
	DebugOut(L"[INFO] Bye bye \n");
}

void CGame::Draw(float x, float y, int xCenter, int yCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha, D3DXVECTOR2 scale, float rotation)
{
	D3DXMATRIX oldMatrix, newMatrix;
	spriteHandler->GetTransform(&oldMatrix);

	D3DXVECTOR3 p(x, y, 0);
	D3DXVECTOR3 pCenter(xCenter, yCenter, 0);

	D3DXMatrixTransformation2D(&newMatrix, new D3DXVECTOR2(x,y), 0.0f, &scale, NULL, D3DXToRadian(rotation), NULL);

	spriteHandler->SetTransform(&newMatrix);

	// Vẽ bằng tâm
	spriteHandler->Draw(texture, &rect, &pCenter, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	spriteHandler->SetTransform(&oldMatrix);
}

// scale với flip đc nhưng k thể draw map đc do khi truyền vô tham số scale để là 0 :D nên k thấy gì hết. ph set là 1
void CGame::Draw(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha, D3DXVECTOR2 scale, float rotation)
{
	d3ddv->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//DebugOut(L"Start Drawing.. \n");

	D3DXVECTOR3 pos(position.x, position.y, 0); // Muốn flip thì không chỉ scale -1 mà còn phải position -1 nữa
	D3DXVECTOR3 pCenter(pointCenter.x, pointCenter.y, 0);

	// spriteHandlerkhi vẽ sẽ có 1 transform matrix
	// sẽ vẽ ra textue như thế nào tùy thuộc vào matrix đó
	// do đó ta sẽ có old matrix và new matrix
	D3DXMATRIX oldMatrix, newMatrix;
	spriteHandler->GetTransform(&oldMatrix);

	//DebugOut(L"Scale.x: %f \n", scale.x);

	// Trước khi vẽ mình set cái matrix transform. Sau khi vẽ xong mình trả lại transform trước đó
	D3DXMatrixTransformation2D(&newMatrix, &position, 0.0f, &scale, NULL, D3DXToRadian(rotation), NULL);
	//D3DXMatrixTransformation2D(&newMatrix, &position, 0.0f, NULL, NULL, D3DXToRadian(0.0f), NULL);

	spriteHandler->SetTransform(&newMatrix);


	// Vẽ bằng tâm
	D3DXVECTOR3 pCen = D3DXVECTOR3(pCenter.x, pCenter.y, 0);
	spriteHandler->Draw(texture, &rect, &pCen, &pos, D3DCOLOR_ARGB(alpha, 255, 255, 255));

	spriteHandler->SetTransform(&oldMatrix);

}

void CGame::Render()
{
	D3DCOLOR bgColor = D3DCOLOR_XRGB(0, 0, 0);
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene != nullptr)
		bgColor = activeScene->GetBackgroundColor();
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, bgColor, 1.0f, 0);

	if (d3ddv->BeginScene())
	{
		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

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

/*
	Standard sweptAABB implementation
	Source: GameDev.net
*/
void CGame::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -999999.0f;
		tx_exit = 999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999.0f;
		ty_exit = 99999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}
