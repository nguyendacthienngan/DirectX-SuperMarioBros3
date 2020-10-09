#pragma once
#ifndef SCENE1_H
#define SCENE1_H

#include "Scene.h"
#include "Mario.h"

class CScene1: public CScene
{
private:
	CMario* mario;
public:
	CScene1();

	void KeyState(BYTE* state);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);

	void Load();
	//void LoadMap(eType);


	~CScene1();
};

#endif