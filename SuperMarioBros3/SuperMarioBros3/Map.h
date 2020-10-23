#pragma once

#ifndef MAP_H
#define MAP_H
#include "./MapReader/Tmx.h.in"
#include "Sprite.h"
#include "Camera.h"
#include "GameObject.h"
#include <string>
#include <map>
#include "TileMap.h"

class CMap
{
private:
	CTileMap* tileMap;
	std::vector<LPGameObject> listGameObjects;
public:
	CMap(std::string filePath);
	~CMap();
	void LoadTilemap(std::string filePath);// tạo đối tương Map và khởi tạo các tileset từ thông tin map nhận được
	
	void Update(CCamera* camera, DWORD dt);

	// vẽ map lên màn hình. Tính toán vùng càn vẽ, vị trí tile trong tileset để vẽ trong hàm
	void Render(CCamera* camera);
	std::vector<LPGameObject> GetListGameObjects();
};

#endif
