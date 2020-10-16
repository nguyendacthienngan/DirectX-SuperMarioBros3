#pragma once

#ifndef MAP_H
#define MAP_H
#include "./MapReader/Tmx.h.in"
#include "Sprite.h"
#include "Camera.h"
#include "TileMap.h"

#include <string>
#include <map>

class CMap
{
private:
	//Tmx::Map* map;
	CTileMap* tileMap;
	std::map<int, LPSprite>  listTileset;

public:
	CMap(std::string filePath, std::string fileName);
	~CMap();
	//Tmx::Map* GetMap(); // trả về thông tin của map

	//void LoadMap(std::string filePath); // tạo đối tương Map và khởi tạo các tileset từ thông tin map nhận được
	// vẽ map lên màn hình. Tính toán vùng càn vẽ, vị trí tile trong tileset để vẽ trong hàm
	//void Draw();  // vẽ toàn bộ map
	//void Draw(CCamera* camera); // vẽ những gì cần load lên camera thui

	void LoadTilemap(std::string filePath, std::string fileName);
	void Update(CCamera* camera, DWORD dt);
	//void Render(CCamera* camera);
	void Render(CCamera* camera);

	//int GetWidth(); // Chiều rộng map (theo số tile)
	//int GetHeight(); //Chiều dài map(theo số tile)
	//int GetTileWidth(); // Chiều rộng của tileset [Hiện tại: 48x48]
	//int GetTileHeight();// Chiều rộng của tileset

	void SetCamera();
};

#endif
