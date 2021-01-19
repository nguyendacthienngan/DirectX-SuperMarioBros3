#pragma once
#include "Index.h"
#include "GameObject.h"
#include <unordered_set>
struct Index;
class CCell
{
private:
	Index index;
	RECT rect;
	std::unordered_set<CGameObject*> gameObjects;
public:
	CCell();
	CCell(Index index);
	void AddObject(CGameObject* gO);
	void RemoveObject(CGameObject* gO);
	std::unordered_set<CGameObject*> GetListGameObject();
	~CCell();
};

