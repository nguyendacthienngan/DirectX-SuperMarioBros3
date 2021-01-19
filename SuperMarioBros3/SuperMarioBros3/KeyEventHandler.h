#pragma once
#include <Windows.h> // IMPORTANT
#include <minwindef.h> 
#include <vector>

#include "GameObject.h"
#ifndef KEYEVENTHANDLER_H
#define KEYEVENTHANDLER_H

class CKeyEventHandler;
typedef CKeyEventHandler* LPKeyEventHandler;
class CKeyEventHandler
{
public:
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	virtual void KeyState() = 0;
};

#endif