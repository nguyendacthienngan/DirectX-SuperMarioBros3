#pragma once
#include "Scene.h"
class CIntro :
    public CScene
{

public:
	CIntro();
	void Load();
	void Unload() override;
};

