#include "Grid.h"
#include "GridConst.h"
using namespace std;
CGrid::CGrid()
{
}

CGrid::CGrid(D3DXVECTOR2 mapSize, std::vector<CGameObject*>gameObjects)
{
	columns = (int) mapSize.x / (int) CELL_WIDTH;
	rows = (int) mapSize.y / (int) CELL_HEIGHT;

	// Khởi tạo cells
	for (int i = 0; i < columns; i++)
	{
		auto column = std::vector<CCell*>();
		for(int j = 0; j < rows ; j++)
		{
			column.push_back(new CCell(D3DXVECTOR2(i, j)));
		}
		cells.push_back(column);
	}
}

void CGrid::Insert(CGameObject* gameObject)
{
	if (gameObject == NULL)
		return;
	auto objSize = CGameObject::GetGameObjectSize(gameObject);
	auto pos = gameObject->GetPosition();
	RECT rect = GetRectByPosition(pos, objSize);

	for (int i = rect.top; i <= rect.bottom; i++)
	{
		for (int j = rect.left; j <= rect.right; j++)
		{
			cells[i][j]->AddObject(gameObject);
		}
	}
}

void CGrid::Move(D3DXVECTOR2 oldPosition, CGameObject* gameObject, D3DXVECTOR2 size)
{
	auto oldRect = GetRectByPosition(oldPosition, size);
	auto newRect = GetRectByPosition(gameObject->GetPosition(), size);
	if (RectEqual(oldRect, newRect) == true)
	{
		return;
	}

	for (int i = oldRect.top; i <= oldRect.bottom; i++)
	{
		for (int j = oldRect.left; j <= oldRect.right; j++)
		{
			cells[i][j]->RemoveObject(gameObject);
		}
	}

	Insert(gameObject);
}

RECT CGrid::GetRectByPosition(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	RECT rect;
	rect.left = (int)pos.x / CELL_WIDTH;
	rect.right = (int)(pos.x + size.x) / CELL_WIDTH;
	rect.top = (int)pos.y / CELL_HEIGHT;
	rect.bottom = (int)(pos.y + size.y) / CELL_HEIGHT;
	return rect;
}

CGrid::~CGrid()
{
}
