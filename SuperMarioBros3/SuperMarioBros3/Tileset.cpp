#include "Tileset.h"
#include "TextureManager.h"
#include "Game.h"
using namespace std;

CTileset::CTileset(int firstgid, D3DXVECTOR2 tileSize, int tileCount, int columns, std::string imageSource)
{
	this->firstgid = firstgid;
	this->tileSize = tileSize;
	this->tileCount = tileCount;
	this->columns = columns;
	CTextureManager::GetInstance()->Add(std::to_string(firstgid), ToLPCWSTR(imageSource), D3DCOLOR_ARGB(0, 0, 0, 0));
	this->texture = CTextureManager::GetInstance()->GetTexture(std::to_string(firstgid));
}

CTileset::CTileset(TiXmlElement* data, std::string xmlSource)
{
	data->QueryIntAttribute("firstgid", &this->firstgid);
	data->QueryFloatAttribute("tilewidth", &this->tileSize.x);
	data->QueryFloatAttribute("tileheight", &this->tileSize.y);
	data->QueryIntAttribute("tilecount", &this->tileCount);
	data->QueryIntAttribute("columns", &this->columns);

	TiXmlElement* imgDom = data->FirstChildElement("image");
	string imgPath = xmlSource + imgDom->Attribute("source");
	CTextureManager::GetInstance()->Add(std::to_string(firstgid), ToLPCWSTR(imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));
	this->texture = CTextureManager::GetInstance()->GetTexture(std::to_string(firstgid));
}

CTileset::~CTileset()
{
	if (texture != NULL) delete texture;
	texture = NULL;
}

void CTileset::Draw(int gid, D3DXVECTOR2 position, D3DCOLOR overlay)
{
	if (gid < firstgid) return; // ?
	RECT r;
	r.top = ( (gid - firstgid) / columns ) * tileSize.y;
	r.left = ((gid - firstgid) % columns) * tileSize.x;
	r.bottom = r.top + tileSize.y;
	r.right = r.left + tileSize.x;

	CGame::GetInstance()->Draw(position, D3DXVECTOR2(tileSize.x / 2, tileSize.y / 2), texture, r, overlay);
}
