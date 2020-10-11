#include "Map.h"

#include <d3dx9.h>
#include <d3d9.h>

#include "TextureManager.h"
#include "MapConst.h"
#include "Ultis.h"

using namespace std;

CMap::CMap(std::string filePath)
{
	LoadMap(filePath);
}

CMap::~CMap()
{
	DebugOut(L"[INFO] Deleting map \n");

	delete map;
}

Tmx::Map* CMap::GetMap()
{
	return map;
}

void CMap::LoadMap(std::string filePath)
{
	map = new Tmx::Map();
	map->ParseFile(filePath);

	RECT rect;
	rect.left = 0; rect.top = 0;
	rect.right = this->GetWidth();	rect.bottom = this->GetHeight();
	DebugOut(L"[INFO] Map's width: %d", rect.right);
	DebugOut(L"[INFO] Map's height: %d", rect.bottom);

	for (int i = 0; i < map->GetNumTilesets(); i++) // duyệt qua các layers của map
	{
		DebugOut(L"[INFO] Load Tile Layers : %d", i);
		DebugOut(L"\n");

		const Tmx::Tileset *tileset = map->GetTileset(i);

		CTextureManager::GetInstance()->Add(MAP_WORLD1_1_NAME, ToLPCWSTR(tileset->GetImage()->GetSource().c_str()), D3DCOLOR_XRGB(255, 128, 192));
		LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->GetTexture(MAP_WORLD1_1_NAME);
		LPSprite sprite = new CSprite(MAP_WORLD1_1_NAME, RECT(), tex);

		listTileset.insert(make_pair(i, sprite)); // insert vào danh sách tileset. Hiện tại mình chỉ có 1 tileset nhưng mình làm tổng quát để sau này có thêm tileset đỡ phải sửa lại

		
		DebugOut(L"[INFO] Loaded Map Successfully \n");
	}
}

void CMap::Draw()
{
	DebugOut(L"[INFO] Begin drawing map... \n");

	for (int i = 0; i < map->GetNumTileLayers(); i++) // duyệt qua các layers của map
	{
		const Tmx::TileLayer* layer = map->GetTileLayer(i);
		if (layer->IsVisible() == false) // Nếu layer bị hide thì không duyệt
		{
			continue;
		}

		RECT sourceRECT;

		for (int m = 0; m < layer->GetWidth(); m++) // Duyệt qua chiều rộng của layer (thiệt chất là chiều rộng của map luôn do mỗi layer phải có kích thước = map)
		{
			for (int n = 0; n < layer->GetHeight(); n++) // Duyệt qua chiều dài của layer (thiệt chất là chiều dài của map luôn do mỗi layer phải có kích thước = map)
			{
				int tilesetIndex = layer->GetTileTilesetIndex(m, n); // Nghĩa là index của 1 tile ở trong hình tileset sẽ là 1 con số. Khi vào map ta muốn đặt tile đó vào map thì ta lưu con số đó
				if (tilesetIndex != -1) // Do nếu ở map không đặt tile nào thì sẽ đc lưu là -1. Nên ta chỉ xét những số khác -1
				{

					const Tmx::Tileset *tileSet = map->GetTileset(tilesetIndex);
					int tileWidth = map->GetTileWidth(); // 48
					int tileHeight = map->GetTileHeight(); // 48

					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth; // 29
					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight; // 30
					//DebugOut(L"[INFO] Tileset width %d \n", tileSetWidth);
					//DebugOut(L"[INFO] Tileset height %d \n", tileSetHeight);

					//tile index
					int tileID = layer->GetTileId(m, n);
					//DebugOut(L"[INFO] Tileset id %d \n", tileID);
					//giá trị tileid sẽ có giá trị từ 0

					LPSprite sprite = listTileset[layer->GetTileTilesetIndex(m, n)]; // lấy ra tileset mà layer này dùng. Trong trường hợp mình dùng thì chỉ có 1 tileset
					//DebugOut(L"[INFO] Tile tileset index %d \n", layer->GetTileTilesetIndex(m, n)); // 0
					//chú ý là tileindex sẽ có giá trị từ 1

					// lấy ra vị trí x,y (tương đối) của mỗi tile trên map để vẽ 

					int y = tileID / tileSetWidth;   // tile nằm ở hàng thứ mấy VD: 11/6 = 1 => tile nằm ở hàng thứ 1 (hàng đc đánh số từ 0)
					int x = tileID - y * tileSetWidth; // 11 - 1 * 6 = 5 => Tile nằm ở côt thứ 5 (cột cx đánh số từ số 0)
					//DebugOut(L"[INFO] Tile x,y: %d , %d\n", x,y);
					// Vậy với id = 11 thì ứng với index số 12 trên tileset nàm ở hàng thứ 1 và cột thứ 5

					// vị trí thực của tile

					sourceRECT.top = y * tileHeight; 
					sourceRECT.bottom = sourceRECT.top + tileHeight;
					sourceRECT.left = x * tileWidth;
					sourceRECT.right = sourceRECT.left + tileWidth;

					//tru tilewidth/2 va tileheight/2 vi Sprite ve o vi tri giua hinh anh cho nen doi hinh de cho
					//dung toa do (0,0) cua the gioi thuc la (0,0) neu khong thi se la (-tilewidth/2, -tileheigth/2);
					D3DXVECTOR3 position(m * tileWidth + tileWidth / 2, n * tileHeight + tileHeight / 2, 0);
					sprite->SetWidth(tileWidth);
					sprite->SetHeight(tileHeight);

					// Test
					sprite->SetRect(sourceRECT);
					//DebugOut(L"[INFO] RECT.left %d \n", sprite->GetRect().left);

					D3DXVECTOR2 pos = D3DXVECTOR2(position.x, position.y);
					D3DXVECTOR2 vector0 = D3DXVECTOR2();
					sprite->Draw(pos, vector0, 0.0f, D3DCOLOR_XRGB(255, 128, 192)); // truyền vô source Rect how???

				}
			}
		}
	}
	DebugOut(L"[INFO] Drawed map sucessfully. \n");

}

int CMap::GetWidth()
{
	return map->GetTileWidth() * map->GetWidth();
}

int CMap::GetHeight()
{
	return map->GetTileHeight() * map->GetHeight();
}

int CMap::GetTileWidth()
{
	return map->GetTileWidth();
}

int CMap::GetTileHeight()
{
	return map->GetTileHeight();
}
