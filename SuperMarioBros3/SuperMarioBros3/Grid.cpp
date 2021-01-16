#include "Grid.h"
#include "GridConst.h"

CGrid::CGrid()
{
}

CGrid::CGrid(D3DXVECTOR2 mapSize, std::vector<CGameObject*>gameObjects)
{
	columns = (int) mapSize.x / (int) CELL_WIDTH;
	rows = (int) mapSize.y / (int) CELL_HEIGHT;

	// Khởi tạo cells
	for (int i = 0; i < rows; i++)
	{
		auto column = std::vector<CCell*>();
		for(int j = 0; j < columns ; j++)
		{
			column.push_back(new CCell(D3DXVECTOR2(i, j)));
		}
		cells.push_back(column);
	}
}

void CGrid::Insert(CGameObject* gameObject)
{
	RECT rect;
	auto x = gameObject->GetPosition().x;
	auto y = gameObject->GetPosition().y;
	rect.left = (int) x / CELL_WIDTH;
	//rect.right = (int) (x + gameObject->GetCollisionBox() / CELL_WIDTH;
	rect.left = (int) x / CELL_WIDTH;
	rect.left = (int) x / CELL_WIDTH;
}

CGrid::~CGrid()
{
}
