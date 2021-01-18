#pragma once
#include "KeyEventHandler.h"
#include "GameConst.h"
#include <dinput.h>
#include "GameObject.h"

class CKeyboardManager;
typedef CKeyboardManager* LPKeyboardManager;

class CKeyboardManager
{
private:
	static LPKeyboardManager instance;

	LPDIRECTINPUT8       dInput;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 dInputDevice;		// Device có thể là là bất kỳ thiết bị nào. Nhưng trg game của mình thì chỉ có keyboard nên đây là Keyboard device 

	LPKeyEventHandler keyEventHandler;

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data
	HWND hWnd;

	// OnKeyDown: Event
	// GetKeyDown: State

public:
	CKeyboardManager();
	~CKeyboardManager();

	static LPKeyboardManager GetInstance();

	void InitKeyboard(LPKeyEventHandler handler);

	void ProcessKeyboard(); // Khi Game đang chạy ta sẽ gọi Process keyboard liên tục để kiểm tra
	bool CheckESCKey();

	//BYTE GetKeyState(); //TO-DO Còn key state khi ấn giữ thì sao?? 
	bool GetKeyStateDown(int keyCode);
	bool GetKeyStateUp(int keyCode);

	void SetHWND(HWND h);
	//void SetKeyEventHandler(LPKeyEventHandler k);

	LPKeyEventHandler GetKeyEventHandler() { return this->keyEventHandler; }
	void AddTargetForKeyboard(CGameObject* target);

};

