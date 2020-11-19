#pragma once
#include "GameObject.h"
class CObjectPool
{
	std::vector<CGameObject*> pool;
public:
	void Add(CGameObject* obj);
	CGameObject* Init();
	void Revoke(CGameObject* obj);
	bool IsEmpty();
	void AddPoolToScene(CScene* scene);
};

