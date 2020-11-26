#pragma once
#include "Scene.h"
#include "Mario.h"
class CWorldMap1: public CScene
{
private:
	CMario* mario;
public:
	CWorldMap1();

	void Load();
	void Unload() override;

	~CWorldMap1();
};

