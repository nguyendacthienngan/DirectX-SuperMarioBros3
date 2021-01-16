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
	~CGrid();
};

