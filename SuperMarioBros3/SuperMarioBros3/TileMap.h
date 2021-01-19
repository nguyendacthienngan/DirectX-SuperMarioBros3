#pragma once
#include "Camera.h"

#include "tinyxml.h"
#include <map>
#include <vector>
#include "TextureManager.h"
#include "Graph.h"
#include "Scene.h"

struct TileSet;
struct Layer;
class CGraph;
class CObjectPool;
class CGrid;
class CScene;

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
	CGrid* grid;
	std::vector<CGameObject*>bricks, coins;
	CObjectPool* poolBricks;
	CObjectPool* poolCoins;

	CGameObject* card;
	CGameObject* player;
	CScene* scene;

	std::unordered_map<int, CGameObject*> objects;
	std::unordered_map<int, std::vector<CGameObject*>> objectGroups;
public:
	CTileMap();
	CTileMap(int width, int height, int tileWidth, int tileHeight);

	TileSet* GetTileSetByTileID(int id);
	CTileMap* LoadMap(std::string filePath, std::string fileMap, std::vector<LPGameObject>& listGameObjects, CGameObject* player, CScene* scene);
	Layer* LoadLayer(TiXmlElement* layerElement);

	void LoadSolidBox(D3DXVECTOR2 position, D3DXVECTOR2 size, std::string name, std::vector<LPGameObject>& listGameObjects);
	void LoadGhostBox(D3DXVECTOR2 position, D3DXVECTOR2 size, std::string name, std::vector<LPGameObject>& listGameObjects);
	
	void LoadEnemy(D3DXVECTOR2 position, std::string enemyName, std::string enemyType, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects);
	void LoadKoopa(D3DXVECTOR2 position, std::string enemyType, std::vector<LPGameObject>& listGameObjects);
	void LoadParakoopa(D3DXVECTOR2 position, std::string enemyType, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects);
	void LoadGoomba(D3DXVECTOR2 position, std::string enemyType, std::vector<LPGameObject>& listGameObjects);
	void LoadParagoomba(D3DXVECTOR2 position, std::string enemyType, std::vector<LPGameObject>& listGameObjects);
	void LoadPiranha(D3DXVECTOR2 position, std::vector<LPGameObject>& listGameObjects);
	void LoadVenus(D3DXVECTOR2 position, std::string enemyType, std::vector<LPGameObject>& listGameObjects);

	void LoadQuestionBlock(D3DXVECTOR2 position, int type, std::string name, std::vector<LPGameObject>& listGameObjects);
	void LoadBrick(D3DXVECTOR2 position, int type, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects);
	void LoadCoin(D3DXVECTOR2 position, int type, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects);

	void LoadPortal(D3DXVECTOR2 position, D3DXVECTOR2 size, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects);
	void LoadLabel(D3DXVECTOR2 position, std::string labelName, D3DXVECTOR2 size, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects);
	
	void LoadWorldItem(D3DXVECTOR2 position, std::string itemName, std::vector<LPGameObject>& listGameObjects);
	void LoadPortalScene(D3DXVECTOR2 position, D3DXVECTOR2 size, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects);
	void LoadSceneGate(D3DXVECTOR2 position, D3DXVECTOR2 size, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects);
	void LoadNodeGate(D3DXVECTOR2 position, D3DXVECTOR2 size, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects);
	
	void LoadSwitchBlock(D3DXVECTOR2 position, std::vector<LPGameObject>& listGameObjects);
	void LoadEmptyBlock(D3DXVECTOR2 position, std::vector<LPGameObject>& listGameObjects);
	void LoadCard(D3DXVECTOR2 position, std::vector<LPGameObject>& listGameObjects);
	void LoadMovingPlatform(D3DXVECTOR2 position, std::vector<LPGameObject>& listGameObjects);

	void Render(CCamera* camera, bool isRenderForeground);
	void RenderLayer(Layer* layer, int i, int j, int x, int y);
	CGraph* GetGraph();

	// Bricks và Coins đặc biệt là do nó chịu sự quản lý của Switch Button => Sau khi Switch Button chuyển trạng thái, Brick sẽ chuyển thành coin 
	// => Ta lấy coin từ poolCoins. Hết thời gian thì Coin chuyển thành Brick, ta dùng cách ngược lại
	std::vector<CGameObject*> GetBricks();
	std::vector<CGameObject*> GetCoins();
	CObjectPool* GetPoolBricks();
	CObjectPool* GetPoolCoins();
	CGameObject* GetCard();

	void AddObjectToList(CGameObject* gO);
	void AddObjectToList(CGameObject* gO, std::vector<LPGameObject>& listGameObjects);
	CGrid* GetGrid();
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