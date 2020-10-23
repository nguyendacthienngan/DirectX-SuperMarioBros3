#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "./tinyxml.h"

class CTileset;
typedef CTileset* LPTileset;
class CTileset
{
private:
	//// firstgid là cái GID của tileset ĐẦU TIÊN
	// FirstGID luôn là 1
	// Và gid của tileset sau sẽ = firstGID + số tiles của tileset trước đó
	// VD: Tileset đầu có 300 tiles => thì GID của tileset sau sẽ là 301
	// Như vậy ta có thể dùng gid trong file tmx làm tileid luôn

	// Lưu ý là có 2 khái niệm tileid trong file tilemap
	// 1 là cái xét trong tileset: luôn bắt đầu bằng 0  (tileID)
	// 1 là cái xét trong layers: tileId + firstGid của tileset tương ứng (tileID + 1)

	int firstgid; // Có thể dùng để đại diện tileset (ID tileset)
	D3DXVECTOR2 tileSize;
	int tileCount;
	int columns;
	LPDIRECT3DTEXTURE9 texture;

public:
	CTileset(int firstgid, D3DXVECTOR2 tileSize, int tileCount, int columns, std::string imageSource);
	CTileset(TiXmlElement* data, std::string xmlSource);
	~CTileset();
	int GetFirstgid() { return firstgid; }

	void Draw(int gid, D3DXVECTOR2 position, D3DCOLOR overlay = D3DCOLOR_ARGB(255, 255, 255, 255));
};