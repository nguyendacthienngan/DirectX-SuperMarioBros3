#include "Cell.h"
#include "GridConst.h"
using namespace std;

CCell::CCell()
{
}

CCell::CCell(D3DXVECTOR2 index)
{
	this->index = index;
	/*rect.left = index.x;
	rect.top = index.y;
	rect.right = index.x + cellWidth;
	rect.bottom = index.y + cellHeight;*/
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
