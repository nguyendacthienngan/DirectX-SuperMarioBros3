#include "Cell.h"
#include "GridConst.h"
CCell::CCell(D3DXVECTOR2 index)
{
	this->index = index;
	/*rect.left = index.x;
	rect.top = index.y;
	rect.right = index.x + cellWidth;
	rect.bottom = index.y + cellHeight;*/
	cellWidth = CELL_WIDTH;
	cellHeight = CELL_HEIGHT;
}

void CCell::AddObject(CGameObject* gO)
{
	// Nếu trong danh sách gameObjects không có gO thì mới add vào, tránh trùng lặp 
	if (gameObjects.find(gO) == gameObjects.end())
	{
		gameObjects.insert(gO);
	}
}
