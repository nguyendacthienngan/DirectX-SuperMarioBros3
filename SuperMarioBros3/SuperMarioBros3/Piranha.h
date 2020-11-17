#pragma once
#include "Enemy.h"
class CPiranha :
    public CEnemy
{

public:
	CPiranha();
	void Init();
	virtual void LoadAnimation();
};

