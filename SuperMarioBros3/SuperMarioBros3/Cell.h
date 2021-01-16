#pragma once
#include "GameObject.h"
#include <unordered_set>
class CCell
{
private:
	D3DXVECTOR2 index;
	RECT rect;
	int cellWidth;
	int cellHeight;
	unordered_set<CGameObject*> gameObjects;
public:
	CCell(D3DXVECTOR2 index);
	void AddObject(CGameObject* gO);
};

