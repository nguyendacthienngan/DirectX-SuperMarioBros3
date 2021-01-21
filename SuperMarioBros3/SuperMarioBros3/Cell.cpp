#include "Cell.h"
#include "GridConst.h"
using namespace std;

CCell::CCell()
{
}

CCell::CCell(Index index)
{
	this->index = index;
}

void CCell::AddObject(CGameObject* gO)
{
	// Nếu trong danh sách gameObjects không có gO thì mới add vào, tránh trùng lặp 
	if (gameObjects.find(gO) == gameObjects.end())
	{
		gameObjects.insert(gO);
	}
}

void CCell::RemoveObject(CGameObject* gO)
{
	// Nếu có trong ds mới xóa
	if (gameObjects.find(gO) != gameObjects.end())
	{
		gameObjects.erase(gO);
	}
}

std::unordered_set<CGameObject*> CCell::GetListGameObject()
{
	return gameObjects;
}

CCell::~CCell()
{
	for (auto gameObject : gameObjects)
	{
		delete gameObject;
		gameObject = NULL;
	}
	gameObjects.clear();
}
