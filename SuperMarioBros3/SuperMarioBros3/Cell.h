#pragma once
#include "GameObject.h"
#include <unordered_set>
class CCell
{
private:
	D3DXVECTOR2 index;
	RECT rect;
	unordered_set<CGameObject*> gameObjects;
public:
	CCell(D3DXVECTOR2 index);
	void AddObject(CGameObject* gO);
	void RemoveObject(CGameObject* gO);
	std::vector<CGameObject*> GetActiveObjects();
};

