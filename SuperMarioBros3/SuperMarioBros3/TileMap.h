#pragma once
#include "Camera.h"

#include "tinyxml.h"
#include <map>
#include <vector>
#include "TextureManager.h"
#include "Graph.h"
struct TileSet;
struct Layer;
class CGraph;
class CObjectPool;
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
	CGraph* graph;

	std::vector<CGameObject*>bricks, coins;
	CObjectPool* poolBricks;
	CObjectPool* poolCoins;
public:
	CTileMap();
	CTileMap(int width, int height, int tileWidth, int tileHeight);

	TileSet* GetTileSetByTileID(int id);
	void Render(CCamera* camera, bool isRenderForeground);
	static CTileMap* LoadMap(std::string filePath, std::string fileMap, std::vector<LPGameObject>& listGameObjects);
	Layer* LoadLayer(TiXmlElement* layerElement);
	void RenderLayer(Layer* layer, int i, int j, int x, int y);
	CGraph* GetGraph();

	// Bricks và Coins đặc biệt là do nó chịu sự quản lý của Switch Button => Sau khi Switch Button chuyển trạng thái, Brick sẽ chuyển thành coin 
	// => Ta lấy coin từ poolCoins. Hết thời gian thì Coin chuyển thành Brick, ta dùng cách ngược lại
	std::vector<CGameObject*> GetBricks();
	std::vector<CGameObject*> GetCoins();
	CObjectPool* GetPoolBricks();
	CObjectPool* GetPoolCoins();

	~CTileMap();
};

struct TileSet
{
	int firstgid; // Có thể dùng để đại diện tileset (ID tileset)
	D3DXVECTOR2 tileSize;
	int tileCount;
	int columns;
	LPDIRECT3DTEXTURE9 texture;
	std::string textureID;
	void Clear()
	{
		CTextureManager::GetInstance()->ClearTextureById(textureID);
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