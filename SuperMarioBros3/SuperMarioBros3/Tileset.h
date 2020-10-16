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
	int firstgid; // Có thể dùng để đại diện tileset
	D3DXVECTOR2 tileSize;
	int tileCount;
	int columns;
	LPDIRECT3DTEXTURE9 texture;

public:
	CTileset(int firstgid, D3DXVECTOR2 tileSize, int tileCount, int columns, std::string imageSource);
	CTileset(TiXmlElement* data, std::string xmlSource);
	~CTileset();
	int GetFirstgid() { return firstgid; }

	void Draw(int gid, D3DXVECTOR2 position, D3DCOLOR overlay = D3DCOLOR_ARGB(255,255,255,255));
};

