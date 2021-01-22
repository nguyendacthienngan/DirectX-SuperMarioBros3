#pragma once
#include "Scene.h"
class CScene4 : public CScene
{
public: 
	CScene4();
	void Load();
	void Unload() override;
	void Update(DWORD dt) override; // dt để xác định t va chạm 
	~CScene4();
};

