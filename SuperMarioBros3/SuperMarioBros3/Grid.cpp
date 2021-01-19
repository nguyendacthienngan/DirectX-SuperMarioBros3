#include "Grid.h"
#include "GridConst.h"
#include "Const.h"
using namespace std;
CGrid::CGrid()
{
}

CGrid::CGrid(D3DXVECTOR2 mapSize)
{
	columns = (int)ceil((float)mapSize.x / (float)CELL_WIDTH);
	rows = (int)ceil((float)mapSize.y / (float)CELL_WIDTH);

	// Khởi tạo cells
	for (int i = 0; i < columns; i++)
	{
		auto column = std::vector<CCell*>();
		for(int j = 0; j < rows ; j++)
		{
			column.push_back(new CCell(Index({ i, j })));
		}
		cells.push_back(column);
	}
}

void CGrid::Insert(CGameObject* gameObject)
{
	if (gameObject == NULL)
		return;
	auto pos = gameObject->GetPosition();

	/*
	// 1 object thuộc nhiều cell
	auto objSize = CGameObject::GetGameObjectSize(gameObject);
	RECT rect = GetRectByPosition(pos, objSize);

	for (int i = rect.top; i <= rect.bottom; i++)
	{
		for (int j = rect.left; j <= rect.right; j++)
		{
			cells[i][j]->AddObject(gameObject);
		}
	}*/
	auto cell = GetCell(pos);
	cell->AddObject(gameObject);
}

void CGrid::Remove(CGameObject* gameObject)
{
	if (gameObject == NULL)
		return;
	auto pos = gameObject->GetPosition();
	/*
	// 1 object thuộc nhiều cell
	auto objSize = CGameObject::GetGameObjectSize(gameObject);
	RECT rect = GetRectByPosition(pos, objSize);

	for (int i = rect.top; i <= rect.bottom; i++)
	{
		for (int j = rect.left; j <= rect.right; j++)
		{
			cells[i][j]->RemoveObject(gameObject);
		}
	}*/
	// 1 object thuộc 1 cell
	auto cell = GetCell(pos);
	cell->RemoveObject(gameObject);
}

void CGrid::Move(D3DXVECTOR2 oldPosition, CGameObject* gameObject)
{
	if (gameObject == NULL)
		return;
	/*
	// 1 object thuộc nhiều cell
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
	}*/

	// 1 object thuộc 1 cell
	auto oldCell = GetCellIndexByPosition(oldPosition);

	auto newCell = GetCellIndexByPosition(gameObject->GetPosition());
	if (oldCell.x != newCell.x || oldCell.y != newCell.y)
	{
		auto cell = GetCell(oldCell);
		cell->RemoveObject(gameObject);
		Insert(gameObject);
	}
}

void CGrid::Update(CCamera* camera)
{
	//FindActiveCells(camera);
	//FindActiveGameObjects(camera);
}

std::vector <CCell*>  CGrid::FindActiveCells(CCamera* camera)
{
	activeCells.clear();
	auto viewport = camera->GetPositionCam();
	auto width = camera->GetWidthCam();
	auto height = camera->GetHeightCam();

	int startCellX = (int)(viewport.x / (float)CELL_WIDTH);
	int startCellY = (int)(viewport.y / (float)CELL_HEIGHT);
	int endCellX =	(int)( (viewport.x + width) / (float)CELL_WIDTH );
	int endCellY = (int)( (viewport.y + height) / (float)CELL_HEIGHT );

	for (int i = startCellX -1 ; i <= endCellX + 1 ; i++)
	{
		if (i < 0 || i > columns) continue;
		for (int j = startCellY - 1; j <= endCellY + 1 ; j++)
		{
			if (j < 0 || j > rows) continue;
			auto cell = GetCell(Index({i, j}));
			activeCells.push_back(cell);
		}
	}
	return activeCells;
}

std::vector <CGameObject*>  CGrid::FindActiveGameObjects(CCamera* camera)
{
	vector<CGameObject*> activeObjects;
	for (auto activeCell : activeCells)
	{
		for (auto gameObject : activeCell->GetListGameObject())
		{
			activeObjects.push_back(gameObject);
		}
	}
	return activeObjects;
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

Index CGrid::GetCellIndexByPosition(D3DXVECTOR2 pos)
{
	return { (int) (pos.x / CELL_WIDTH), (int) (pos.y / CELL_HEIGHT) };
}

CCell* CGrid::GetCell(Index index)
{
	if (index.x < 0)
		index.x = 0;
	if (index.x > columns - 1)
		index.x = columns - 1;

	if (index.y < 0)
		index.y = 0;
	if (index.y > rows - 1)
		index.y = rows - 1;
	return cells.at(index.x).at(index.y); // IMPORTANT
}

CCell* CGrid::GetCell(D3DXVECTOR2 position)
{
	auto index = GetCellIndexByPosition(position);
	return GetCell(index);
}

CGrid::~CGrid()
{
	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			delete cells.at(i).at(j);
		}
	}
	cells.clear();
}
