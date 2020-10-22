// SuperMarioBros3.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Const.h"
#include "Game.h"
#include "Ultis.h"
#include "tinyxml.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
bool                LoadFileConfig(int& fps, int& screenWidth, int& screenHeight);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    int fps, screenWidth, screenHeight;
    if (LoadFileConfig(fps, screenWidth, screenHeight) == false)
        return NULL;
    //DebugOut(L"FPS: %d, ScWidth: %d, ScHeight: %d", fps, screenWidth, screenHeight);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    HWND hWnd = InitInstance(hInstance, nCmdShow);
    if (!hWnd)
    {
        return NULL;
    }

    CGame::GetInstance()->InitDirectX(hWnd, screenWidth, screenHeight, fps);
    CGame::GetInstance()->Init();
    CGame::GetInstance()->Run();

    return 0;
}

bool LoadFileConfig(int& fps, int& screenWidth, int& screenHeight)
{
    CGame::GetInstance()->ImportGameSource();
    auto configFilePath = CGame::GetInstance()->GetFilePathByCategory(CATEGORY_CONFIG, CG_GLOBAL_CONFIG);
    
    TiXmlDocument doc(configFilePath.c_str());
    if (doc.LoadFile() == false)
    {
        OutputDebugStringW(ToLPCWSTR(doc.ErrorDesc()));
        return FALSE;
    }
    TiXmlElement* root = doc.RootElement();
    for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        std::string name = element->Attribute("name");
        if (name.compare("frame-rate") == 0)
            element->QueryIntAttribute("value", &fps);
        else if (name.compare("resolution") == 0)
        {
            element->QueryIntAttribute("width", &screenWidth);
            element->QueryIntAttribute("height", &screenHeight);
        }
    }
    DebugOut(L"conf: %d, %d, %d\n", fps, screenWidth, screenHeight);
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = TITLE;
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}

HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(TITLE, TITLE, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return NULL;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        CGame::GetInstance()->End();
        PostQuitMessage(0);
        break;
    //case WM_KEYDOWN: // hỏi thầy
    //    switch (wParam)
    //    {
    //    case VK_ESCAPE:
    //        CGame::GetInstance()->End();
    //        PostQuitMessage(0);
    //        break;
    //    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
