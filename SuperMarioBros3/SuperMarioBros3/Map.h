#pragma once

#ifndef MAP_H
#define MAP_H
#include "./MapReader/Tmx.h.in"
#include "Sprite.h"

#include <string>
#include <map>

class CMap
{
private:
	Tmx::Map* map;
	std::map<int, LPSprite>  listTileset;

public:
	CMap(std::string filePath);
	~CMap();
	Tmx::Map* GetMap(); // trả về thông tin của map

	void LoadMap(std::string filePath); // tạo đối tương Map và khởi tạo các tileset từ thông tin map nhận được
	void Draw(); // vẽ map lên màn hình. Tính toán vùng càn vẽ, vị trí tile trong tileset để vẽ trong hàm

	int GetWidth(); // Chiều rộng map (theo số tile)
	int GetHeight(); //Chiều dài map(theo số tile)
	int GetTileWidth(); // Chiều rộng của tileset [Hiện tại: 48x48]
	int GetTileHeight();// Chiều rộng của tileset

};

#endif
