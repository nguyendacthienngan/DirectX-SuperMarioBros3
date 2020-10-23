#pragma once
#include "Tileset.h"
#include "Layer.h"
#include "Camera.h"

#include <map>
#include <vector>
class CTileMap
{
private:
	int width;
	int height;
	int tileWidth;
	int tileHeight;

	std::map<int, LPTileset> listTilesets;
	std::vector<LPLayer> listLayers;
public:
	CTileMap();
	CTileMap(int width, int height, int tileWidth, int tileHeight);
	LPTileset GetTileSetByTileID(int id);

	void AddTileSet(int firstgid, LPTileset tileSet);

	void AddLayer(LPLayer layer);

	//void Update(int dt);

	void Render(CCamera* camera);

	static CTileMap* LoadMap(std::string filePath, std::vector<LPGameObject>& listGameObjects);

	~CTileMap();
};