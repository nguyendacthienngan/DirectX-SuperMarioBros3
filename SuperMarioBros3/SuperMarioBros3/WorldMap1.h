#pragma once
#include "Scene.h"
#include "Mario.h"
class CWorldMap1: public CScene
{
public:
	CWorldMap1();

	void Load();
	void Unload() override;

	~CWorldMap1();
};

