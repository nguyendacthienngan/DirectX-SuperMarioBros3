#pragma once
#include "Camera.h"

#include "tinyxml.h"
#include <map>
#include <vector>

struct TileSet;
struct Layer;

class CTileMap
{
private:
	int width;
	int height;
	int tileWidth;
	int tileHeight;

	std::map<int, TileSet*> tileSets;
	std::vector<Layer*> layers;
	Layer* foreground;
public:
	CTileMap();
	CTileMap(int width, int height, int tileWidth, int tileHeight);

	TileSet* GetTileSetByTileID(int id);
	void Render(CCamera* camera, bool isRenderForeground);
	static CTileMap* LoadMap(std::string filePath, std::string fileMap, std::vector<LPGameObject>& listGameObjects);
	Layer* LoadLayer(TiXmlElement* layerElement);
	void RenderLayer(Layer* layer, int i, int j, int x, int y);
	~CTileMap();
};

struct TileSet
{
	int firstgid; // Có thể dùng để đại diện tileset (ID tileset)
	D3DXVECTOR2 tileSize;
	int tileCount;
	int columns;
	LPDIRECT3DTEXTURE9 texture;
	void Clear()
	{
		if (texture != NULL) texture->Release();
		texture = NULL;
	}
};

struct Layer
{
	int id;
	int width;
	int height;
	int** tiles; // ma trận
	bool isVisible;
	void Clear()
	{
		for (int i = 0; i < width; i++)
		{
			delete[] tiles[i];
		}
		delete[] tiles;
	}
};