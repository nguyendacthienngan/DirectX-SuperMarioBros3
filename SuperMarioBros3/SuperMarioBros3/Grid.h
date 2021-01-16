#pragma once
#include <vector>
#include "Cell.h"

class CGrid
{
private:
	std::vector<std::vector<CCell*>> cells; // Ma trận các cell
	int columns;
	int rows;
public:
	CGrid();
	CGrid(D3DXVECTOR2 mapSize, std::vector<CGameObject*>gameObjects);
	void Insert(CGameObject* gameObject);
	void Move(D3DXVECTOR2 oldPosition, CGameObject* gameObject, D3DXVECTOR2 size);
	RECT GetRectByPosition(D3DXVECTOR2 pos, D3DXVECTOR2 size);
	std::vector<CGameObject*> GetActiveObjects();
	~CGrid();
};

