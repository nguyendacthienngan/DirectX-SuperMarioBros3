#include "Game.h"
#include "GameConst.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "AnimationManager.h"
#include "Ultis.h"
#include "GameKeyEventHandler.h"

#include "Scene1.h"
#include "tinyxml.h"
#include <string>
#include "WorldMap1.h"
#include "Scene4.h"
#include "Intro.h"
CGame* CGame::instance = NULL;
DWORD CGame::deltaTime = 0;
float CGame::timeScale = 1.0f;

CGame* CGame::GetInstance()
{
    if (instance == NULL) instance = new CGame();
    return instance;
}

CGame::~CGame() 
{
}

void CGame::Init()
{
	DebugOut(L"[INFO] Init Manager \n");
	CTextureManager::GetInstance()->Init();
	CSpriteManager::GetInstance()->Init();
	CAnimationManager::GetInstance()->Init();
	CSceneManager::GetInstance()->Init();

	CGameKeyEventHandler *keyEventHandler = new CGameKeyEventHandler();
	auto keyboardManager = CKeyboardManager::GetInstance();
	keyboardManager->SetHWND(hWnd);
	keyboardManager->InitKeyboard(keyEventHandler);
	
	/*CIntro* intro = new CIntro();
	CSceneManager::GetInstance()->Load(intro);*/

	CScene4* scene4 = new CScene4();
	CSceneManager::GetInstance()->Load(scene4);

	/*CScene1* scene1 = new CScene1();
	CSceneManager::GetInstance()->Load(scene1);*/

	//CWorldMap1* sceneWorld1 = new CWorldMap1();
	//CSceneManager::GetInstance()->Load(sceneWorld1);
	DebugOut(L"[INFO] Init Manager Sucessfully \n");

}

void CGame::Request()
{
	CSceneManager::GetInstance()->LoadRequestScene();
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene != nullptr)
	{
		activeScene->FindUpdateObjects();
	}
}

void CGame::InitDirectX(HWND hWnd, int scrWidth, int scrHeight, int fps)
{
	this->fps = fps;
	this->hWnd = hWnd;
	this->screenWidth = scrWidth;
	this->screenHeight = scrHeight;
	DebugOut(L"[INFO] Begin Init DirectX \n");
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = screenWidth = scrWidth;
	d3dpp.BackBufferHeight = screenHeight = scrHeight;
	d3dpp.Flags = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = NULL;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.hDeviceWindow = hWnd;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,
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

void CGame::Draw(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor)
{
	D3DXVECTOR3 pCenter((int)pointCenter.x, (int)pointCenter.y, 0);
	D3DXVECTOR3 pInt((int)(position.x), (int)(position.y), 0); // Giúp không bị viền
	spriteHandler->Draw(texture, &rect, &pCenter, &pInt, transcolor);
}

void CGame::Draw(D3DXVECTOR2 position, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha)
{
	D3DXVECTOR3 pInt((int)(position.x), (int)(position.y), 0); // Giúp không bị viền
	spriteHandler->Draw(texture, &rect, NULL, &pInt, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

void CGame::Run()
{
	MSG msg;
	bool done = false;

	DWORD frameStart = GetTickCount64();
	DWORD tickPerFrame = 1000 / fps;

	// Game Loop
	while (!done)
	{
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
			DWORD currentTime = GetTickCount64(); // now
			deltaTime = currentTime - frameStart;

			if (deltaTime >= tickPerFrame) // chuyển frame mới
			{
				frameStart = currentTime;
				Request();
				// Process key
				auto keyboardManger = CKeyboardManager::GetInstance();
				keyboardManger->ProcessKeyboard();
				if (keyboardManger->CheckESCKey() == true)
					continue;
				Update();
				Render();
				Clean();
				
				if (deltaTime > tickPerFrame) deltaTime = 0;
			}
			else // chưa tới tickperframe nên cho ngủ vì xong việc cho 1 frame ròi
			{
				Sleep(tickPerFrame - deltaTime);
			}
		}
		
	}
}

void CGame::End()
{
	DebugOut(L"[INFO] This game is about to end \n");

	CSceneManager::GetInstance()->GetActiveScene()->Unload();
	CSceneManager::GetInstance()->GetActiveScene()->DestroyObject();
	CTextureManager::GetInstance()->Clear();
	CSpriteManager::GetInstance()->Clear();
	CAnimationManager::GetInstance()->Clear();

	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
	DebugOut(L"[INFO] Bye bye \n");
}

void CGame::Clean()
{
	CSceneManager::GetInstance()->UnloadRequestScene();
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene != nullptr)
		activeScene->DestroyObject();
}

void CGame::DrawFlipX(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor)
{
	D3DXVECTOR3 pCenter((int)pointCenter.x, (int)pointCenter.y, 0);
	D3DXVECTOR2 pScale(-1, 1);
	D3DXVECTOR3 pInt((int)(position.x), (int)(position.y), 0);
	D3DXMATRIX oldMatrix, newMatrix; 

	spriteHandler->GetTransform(&oldMatrix);

	D3DXMatrixTransformation2D(&newMatrix, &position, 0.0f, &pScale, NULL, 0.0f, NULL);
	spriteHandler->SetTransform(&newMatrix);

	spriteHandler->Draw(texture, &rect, &pCenter, &pInt, transcolor);
	spriteHandler->SetTransform(&oldMatrix);
}

void CGame::DrawFlipY(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor)
{
	D3DXVECTOR3 pCenter((int)pointCenter.x, (int)pointCenter.y, 0);
	D3DXVECTOR2 pScale(1, -1);
	D3DXVECTOR3 pInt((int)(position.x), (int)(position.y), 0);
	D3DXMATRIX oldMatrix, newMatrix;
	
	spriteHandler->GetTransform(&oldMatrix);

	D3DXMatrixTransformation2D(&newMatrix, &position, 0.0f, &pScale, NULL, 0.0f, NULL);
	spriteHandler->SetTransform(&newMatrix);

	spriteHandler->Draw(texture, &rect, &pCenter, &pInt, transcolor);
	spriteHandler->SetTransform(&oldMatrix);
}

void CGame::Render()
{
	D3DCOLOR bgColor = D3DCOLOR_XRGB(0, 0, 0);
	RECT rect = {0, 0, 600, 600};
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	auto uiCamera = CSceneManager::GetInstance()->GetUICamera();
	if (activeScene != nullptr)
		bgColor = activeScene->GetBackgroundColor();
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, bgColor, 1.0f, 0);

	d3ddv->BeginScene();
	spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	if (activeScene != nullptr)
		activeScene->Render();
	if (uiCamera != nullptr)
		uiCamera->Render();
	spriteHandler->End();

	d3ddv->EndScene();
	d3ddv->Present(NULL, NULL, NULL, NULL);

}

void CGame::Update()
{
	LPSceneManager sceneManger = CSceneManager::GetInstance();
	LPScene activeScene = sceneManger->GetActiveScene();
	auto uiCamera = CSceneManager::GetInstance()->GetUICamera();
	// Update Scene. Trong Scene sẽ Update các GameObject. Trong GameObject sẽ update các animation. Các animation sẽ update các animation frame / sprite ?
	if (activeScene != NULL)
		activeScene->Update(deltaTime);
	if (uiCamera != nullptr)
		uiCamera->Update();
}

bool CGame::ImportGameSource()
{
	auto path = "Resources/root.xml";
	TiXmlDocument doc(path);
	if (doc.LoadFile() == false)
	{
		OutputDebugStringW(ToLPCWSTR(doc.ErrorDesc()));
		return false;
	}
	TiXmlElement* root = doc.RootElement();
	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		std::string category = element->Attribute("name");
		OutputDebugStringW(ToLPCWSTR(category + '\n'));

		std::unordered_map<std::string, std::string> bucket;

		for (auto item = element->FirstChildElement(); item != nullptr; item = item->NextSiblingElement())
		{
			std::string id = item->Attribute("id");
			std::string source = item->Attribute("source");
			bucket.insert(make_pair(id, source)); // ******
			OutputDebugStringW(ToLPCWSTR("|--" + id + ":" + source + '\n'));
		}
		gameSource.insert(make_pair(category, bucket));
	}
	return true;
}

std::string CGame::GetFilePathByCategory(std::string category, std::string id)
{
	if (gameSource.find(category) != gameSource.end())
	{
		auto bucket = gameSource.at(category);
		if (bucket.find(id) != bucket.end())
		{
			return bucket.at(id);
		}
		return "";
	}
	return "";
}
