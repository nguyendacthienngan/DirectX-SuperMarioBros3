#pragma once

#ifndef MARIO_H
#define MARIO_H

#include "GameObject.h"
class CMario : public CGameObject
{
public:
	CMario();
	void LoadAnimation();
	void Update();
	~CMario();
};
#endif
