#pragma once
#include "Font.h"
class CMenu
{
private:
	CFont* selection1;
	CFont* selection2;
public:
	CMenu();
	void Update();
	void Render();
};

