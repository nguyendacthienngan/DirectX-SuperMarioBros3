#include "KeyboardManager.h"
#include "Ultis.h"
#include "Game.h"

LPKeyboardManager CKeyboardManager::instance = NULL;

CKeyboardManager::CKeyboardManager()
{
	
}

CKeyboardManager::~CKeyboardManager()
{
	if (dInput != NULL) dInput->Release();
	if (dInputDevice != NULL)
	{
		dInputDevice->Unacquire();
		dInputDevice->Release();
	}
}

LPKeyboardManager CKeyboardManager::GetInstance()
{
	if (instance == NULL) instance = new CKeyboardManager();
	return instance;
}

void CKeyboardManager::InitKeyboard(LPKeyEventHandler handler)
{
	//DebugOut(L"[ERROR] HWND: \n");
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

	//hr = dInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
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
		//hr = dInputDevice->Acquire();

		// Lỗi ở đây, mới vô bị mất acquire
		return;
	}

	this->keyEventHandler = handler; // Truyền đối tượng xử lý từng phím (handler) vào bên trong keyHandler

	DebugOut(L"[INFO] Keyboard has been initialized successfully\n");
}

void CKeyboardManager::ProcessKeyboard()
{
	HRESULT hr;
	// Collect all key states first:Đọc trạng thái của toàn bộ bàn phím. Mỗi một byte ứng với một phím
	hr = dInputDevice->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		CGame::SetTimeScale(0.0f);
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = dInputDevice->Acquire();
			if (h == DI_OK)
			{
				CGame::SetTimeScale(1.0f);
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
		keyEventHandler->KeyState();

	}

}

bool CKeyboardManager::CheckESCKey()
{
	// TODO: Cần chỉnh lại vì nút ESC Chỉ cần nhấn 1 lần
	if (this->GetKeyStateDown(DIK_ESCAPE))
	{
		DebugOutTitle(L"Nhan ESC");
		DebugOut(L"Nhan ESC \n");
		CGame::GetInstance()->End();
		PostQuitMessage(0);
		return true;
	}
	return false;
}

void CKeyboardManager::SetHWND(HWND h)
{
	hWnd = h;
}

bool CKeyboardManager::GetKeyStateDown(int keyCode)
{
	return (keyStates[keyCode] & 0x80) > 0; // Lấy ra nút được ấn
}

bool CKeyboardManager::GetKeyStateUp(int keyCode)
{
	return (keyStates[keyCode] & 0x80) <= 0;
}
