#pragma once
#include "./tinyxml.h"
#include <d3d9.h>
#include <d3dx9.h>

class CLayer;
typedef CLayer* LPLayer;

class CLayer
{
private:
	int id;
	int width;
	int height;
	int** tiles;

public:
	CLayer();
	CLayer(TiXmlElement* data);
	~CLayer();

	int GetTileID(int x, int y);
};

