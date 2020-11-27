#pragma once
#ifndef SCENE1_H
#define SCENE1_H

#include "Scene.h"
#include "Mario.h"

class CScene1: public CScene
{
public:
	CScene1();


	void Load();
	void Unload() override;

	~CScene1();
};

#endif