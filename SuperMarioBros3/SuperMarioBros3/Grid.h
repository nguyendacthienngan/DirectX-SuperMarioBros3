#pragma once
#include <vector>
#include "Cell.h"
class CCell;
class CGrid
{
private:
	std::vector<std::vector<CCell*>> cells; // Ma trận toàn bộ các cell trong map
	int columns;
	int rows;
	RECT rect;
	std::vector <CCell*> activeCells; // các cell được active trong viewport
public:
	CGrid();
	CGrid(D3DXVECTOR2 mapSize);
	void Insert(CGameObject* gameObject);
	void Remove(CGameObject* gameObject);
	void Move(D3DXVECTOR2 oldPosition, CGameObject* gameObject, D3DXVECTOR2 size);
	void Update(CCamera* camera);
	std::vector <CCell*>  FindActiveCells(CCamera* camera);
	std::vector <CGameObject*> FindActiveGameObjects(CCamera* camera);
	RECT GetRectByPosition(D3DXVECTOR2 pos, D3DXVECTOR2 size);
	D3DXVECTOR2 GetCellIndexByPosition(D3DXVECTOR2 pos);

	~CGrid();
};

