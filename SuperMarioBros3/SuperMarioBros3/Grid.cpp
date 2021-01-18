#include "Grid.h"
#include "GridConst.h"
#include "Const.h"
using namespace std;
CGrid::CGrid()
{
}

CGrid::CGrid(D3DXVECTOR2 mapSize)
{
	/*columns = (int) mapSize.x / (int) CELL_WIDTH;
	rows = (int) mapSize.y / (int) CELL_HEIGHT;*/

	columns = (int)ceil((float)mapSize.x / (SCREEN_WIDTH >> 1));
	rows = (int)ceil((float)mapSize.y / (SCREEN_HEIGHT >> 1));

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
	auto currentCell = GetCellIndexByPosition(pos);
	auto x = (int)currentCell.x;
	auto y = (int)currentCell.y;
	cells[x][y]->AddObject(gameObject);
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
	auto currentCell = GetCellIndexByPosition(pos);
	auto x = (int)currentCell.x;
	auto y = (int)currentCell.y;
	cells[x][y]->RemoveObject(gameObject);
}

void CGrid::Move(D3DXVECTOR2 oldPosition, CGameObject* gameObject, D3DXVECTOR2 size)
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
	auto x = (int)oldCell.x;
	auto y = (int)oldCell.y;

	auto newCell = GetCellIndexByPosition(gameObject->GetPosition());
	if (oldCell.x == newCell.x && oldCell.y == newCell.y)
		cells[x][y]->RemoveObject(gameObject);

	Insert(gameObject);
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

	int startCellX = (int)(viewport.x / CELL_WIDTH);
	int startCellY = (int)(viewport.y / CELL_HEIGHT);
	int endCellX =	(int)( (viewport.x + width) / CELL_WIDTH );
	int endCellY = (int)( (viewport.y + height) / CELL_HEIGHT );

	for (int i = startCellX; i <= endCellX; i++)
	{
		if (i < 0 || i > columns) continue;
		for (int j = startCellY; j <= endCellY; j++)
		{
			if (j < 0 || j > columns) continue;
			activeCells.push_back(cells[j][i]);
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

D3DXVECTOR2 CGrid::GetCellIndexByPosition(D3DXVECTOR2 pos)
{
	return { (int)pos.x / CELL_WIDTH, (int) pos.y / CELL_HEIGHT };
}

CGrid::~CGrid()
{
	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			delete cells[i][j];
		}
	}
}
