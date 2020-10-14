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
	DebugOut(L"[INFO] Map's width: %d \n", rect.right);
	DebugOut(L"[INFO] Map's height: %d \n", rect.bottom);

	for (int i = 0; i < map->GetNumTilesets(); i++) // duyệt qua các layers của map
	{
		DebugOut(L"[INFO] Load Tile Layers : %d \n", i);

		const Tmx::Tileset *tileset = map->GetTileset(i);

		CTextureManager::GetInstance()->Add(MAP_WORLD1_1_NAME, ToLPCWSTR(tileset->GetImage()->GetSource().c_str()), D3DCOLOR_XRGB(255, 128, 192));
		LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->GetTexture(MAP_WORLD1_1_NAME);
		LPSprite sprite = new CSprite(MAP_WORLD1_1_NAME, RECT(), tex);

		listTileset.insert(make_pair(i, sprite)); // insert vào danh sách tileset. Hiện tại mình chỉ có 1 tileset nhưng mình làm tổng quát để sau này có thêm tileset đỡ phải sửa lại

		
		DebugOut(L"[INFO] Loaded Map Successfully \n");
	}
}


// Vẽ hết map (Lag cực mạnh)

//void CMap::Draw()
//{
//	DebugOut(L"[INFO] Begin drawing map... \n");
//
//	for (int i = 0; i < map->GetNumTileLayers(); i++) // duyệt qua các layers của map
//	{
//		const Tmx::TileLayer* layer = map->GetTileLayer(i);
//		if (layer->IsVisible() == false) // Nếu layer bị hide thì không duyệt
//		{
//			continue;
//		}
//
//		RECT sourceRECT;
//		/*DebugOut(L"[INFO] layer width %d \n", layer->GetWidth());
//		DebugOut(L"[INFO] layer height %d \n", layer->GetHeight());*/
//
//		for (int m = 0; m < layer->GetWidth(); m++) // Duyệt qua chiều rộng của layer (thiệt chất là chiều rộng của map luôn do mỗi layer phải có kích thước = map)
//		//for (int m = 0; m < 800 / 48; m++)
//		{
//			for (int n = 0; n < layer->GetHeight(); n++) // Duyệt qua chiều dài của layer (thiệt chất là chiều dài của map luôn do mỗi layer phải có kích thước = map)
//			//for (int n = 1248; n < (1248 + 600) / 48; n++)
//			{
//				int tilesetIndex = layer->GetTileTilesetIndex(m, n); // Nghĩa là index của 1 tile ở trong hình tileset sẽ là 1 con số. Khi vào map ta muốn đặt tile đó vào map thì ta lưu con số đó
//				if (tilesetIndex != -1) // Do nếu ở map không đặt tile nào thì sẽ đc lưu là -1. Nên ta chỉ xét những số khác -1
//				{
//
//					const Tmx::Tileset *tileSet = map->GetTileset(tilesetIndex);
//					int tileWidth = map->GetTileWidth(); // 48
//					int tileHeight = map->GetTileHeight(); // 48
//
//					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth; // 29
//					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight; // 30
//					/*DebugOut(L"[INFO] Tileset width %d \n", tileSetWidth);
//					DebugOut(L"[INFO] Tileset height %d \n", tileSetHeight);*/
//
//					//tile index
//					int tileID = layer->GetTileId(m, n);
//					//DebugOut(L"[INFO] Tileset id %d \n", tileID);
//					//giá trị tileid sẽ có giá trị từ 0
//
//					LPSprite sprite = listTileset[layer->GetTileTilesetIndex(m, n)]; // lấy ra tileset mà layer này dùng. Trong trường hợp mình dùng thì chỉ có 1 tileset
//					//DebugOut(L"[INFO] Tile tileset index %d \n", layer->GetTileTilesetIndex(m, n)); // 0
//
//					// lấy ra vị trí x,y (tương đối) của mỗi tile trên map để vẽ 
//
//					int y = tileID / tileSetWidth;   // tile nằm ở hàng thứ mấy VD: 11/6 = 1 => tile nằm ở hàng thứ 1 (hàng đc đánh số từ 0)
//					int x = tileID - y * tileSetWidth; // 11 - 1 * 6 = 5 => Tile nằm ở côt thứ 5 (cột cx đánh số từ số 0)
//					//DebugOut(L"[INFO] Tile x,y: %d , %d\n", x,y);
//					// Vậy với id = 11 thì ứng với index số 12 trên tileset nàm ở hàng thứ 1 và cột thứ 5
//
//					// vị trí thực của tile
//
//					sourceRECT.top = y * tileHeight; 
//					sourceRECT.bottom = sourceRECT.top + tileHeight;
//					sourceRECT.left = x * tileWidth;
//					sourceRECT.right = sourceRECT.left + tileWidth;
//
//					//DebugOut(L"[INFO] SourceRECT: (top, bottom, left, right): (%d, %d, %d, %d) \n", sourceRECT.top, sourceRECT.bottom, sourceRECT.left, sourceRECT.right);
//
//					//tru tilewidth/2 va tileheight/2 vi Sprite ve o vi tri giua hinh anh cho nen doi hinh de cho
//					//dung toa do (0,0) cua the gioi thuc la (0,0) neu khong thi se la (-tilewidth/2, -tileheigth/2);
//					D3DXVECTOR3 position(m * tileWidth + tileWidth / 2, n * tileHeight + tileHeight / 2, 0);
//
//					/*DebugOut(L"[INFO] Tile width %d \n", tileWidth);
//					DebugOut(L"[INFO] Tile height %d \n", tileHeight);*/
//
//					sprite->SetWidth(tileWidth);
//					sprite->SetHeight(tileHeight);
//
//					sprite->SetRect(sourceRECT);
//					// Test
//
//					//DebugOut(L"[INFO] RECT.left %d \n", sprite->GetRect().left);
//
//					D3DXVECTOR2 pos = D3DXVECTOR2(position.x, position.y);
//					D3DXVECTOR2 vectorScale = D3DXVECTOR2(1.0f, 1.0f);
//					sprite->Draw(pos, vectorScale, 0.0f, D3DCOLOR_XRGB(255, 128, 192));
//
//				}
//			}
//		}
//	}
//	DebugOut(L"[INFO] Drawed map sucessfully. \n");
//
//}

// Giải pháp tối ưu: 
// Chỉ loop qua những phần có trong camera
//Nhưng tính toán phức tạp, tạm thời không dùng tới

/*
void CMap::Draw(CCamera* camera)
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

		if (camera->GetWidthCam() <= layer->GetWidth() || camera->GetHeightCam() <= layer->GetHeight())
		{
			DebugOut(L"[INFO] Width height of the cam exceeds the width height map \n");
			return;
		}
		int xCam = (int)camera->GetPositionCam().x;
		int yCam = (int)camera->GetPositionCam().y;
		int widthCam = (int)camera->GetWidthCam();
		int heightCam = (int)camera->GetHeightCam();

		DebugOut(L"[INFO] xCam: %d \n", xCam); // 0
		DebugOut(L"[INFO] yCam: %d \n", yCam); // 1248
		DebugOut(L"[INFO] widthCam: %d \n", widthCam); // 800
		DebugOut(L"[INFO] heightCam: %d \n", heightCam); // 600

		// Chỗ m < xCam + camera -> GetWidthCam() còn bị lỗi. Chưa đúng 
		for (int m = xCam; m < xCam + camera->GetWidthCam(); m++) 
		{
			for (int n = yCam; n < yCam + camera->GetHeightCam(); n++) 
			{
				int tilesetIndex = layer->GetTileTilesetIndex(m, n); // Nghĩa là index của 1 tile ở trong hình tileset sẽ là 1 con số. Khi vào map ta muốn đặt tile đó vào map thì ta lưu con số đó
				DebugOut(L"[INFO] Tileset index: %d \n", tilesetIndex);
				
				if (tilesetIndex != -1) // Do nếu ở map không đặt tile nào thì sẽ đc lưu là -1. Nên ta chỉ xét những số khác -1
				{

					const Tmx::Tileset* tileSet = map->GetTileset(tilesetIndex);
					int tileWidth = map->GetTileWidth(); // 48
					int tileHeight = map->GetTileHeight(); // 48

					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth; // 29
					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight; // 30
					DebugOut(L"[INFO] Tileset width %d \n", tileSetWidth);
					DebugOut(L"[INFO] Tileset height %d \n", tileSetHeight);

					//tile index
					int tileID = layer->GetTileId(m, n);
					DebugOut(L"[INFO] Tileset id %d \n", tileID);
					//giá trị tileid sẽ có giá trị từ 0

					LPSprite sprite = listTileset[layer->GetTileTilesetIndex(m, n)]; // lấy ra tileset mà layer này dùng. Trong trường hợp mình dùng thì chỉ có 1 tileset
					DebugOut(L"[INFO] Tile tileset index %d \n", layer->GetTileTilesetIndex(m, n)); // 0
					//chú ý là tileindex sẽ có giá trị từ 1

					// lấy ra vị trí x,y (tương đối) của mỗi tile trên map để vẽ 

					int y = tileID / tileSetWidth;   // tile nằm ở hàng thứ mấy VD: 11/6 = 1 => tile nằm ở hàng thứ 1 (hàng đc đánh số từ 0)
					int x = tileID - y * tileSetWidth; // 11 - 1 * 6 = 5 => Tile nằm ở côt thứ 5 (cột cx đánh số từ số 0)
					DebugOut(L"[INFO] Tile x,y: %d , %d\n", x,y);
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
					DebugOut(L"[INFO] RECT.left %d \n", sprite->GetRect().left);

					D3DXVECTOR2 pos = D3DXVECTOR2(position.x, position.y);
					D3DXVECTOR2 vector0 = D3DXVECTOR2();
					sprite->Draw(pos, vector0, 0.0f, D3DCOLOR_XRGB(255, 128, 192)); // truyền vô source Rect how???

				}
			}
		}
	}
	DebugOut(L"[INFO] Drawed map sucessfully. \n");

}
*/

void CMap::Update(CCamera* camera, DWORD dt)
{
	if (camera == NULL)
		return;
	camera->Update();
}

// Giải pháp đơn giản:
// Loop qua tất cả các tile
// Chỉ vẽ những tile có trong camera
void CMap::Render(CCamera* camera)
{
	DebugOut(L"[INFO] Begin rendering map... \n");

	if (camera == NULL)
	{
		DebugOut(L"[INFO] Camera is NULL \n");
		return;
	}

	// Lấy ra vị tí của camera
	D3DXVECTOR2 posCam = camera->GetPositionCam();
	// Camera tiến về phía trước thì map phải lùi về phía sau
	D3DXVECTOR2 translation = D3DXVECTOR2(-posCam.x, -posCam.y); // ?
	//DebugOut(L"[INFO] Translation: %f, %f \n", translation.x, translation.y);

	
	int count = 0;

	for (int i = 0; i < map->GetNumTileLayers(); i++) // duyệt qua các layers của map
	{
		const Tmx::TileLayer* layer = map->GetTileLayer(i);
		if (layer->IsVisible() == false) // Nếu layer bị hide thì không duyệt
		{
			continue;
		}

		RECT sourceRECT;

		for (int m = 0; m < layer->GetWidth(); m++)
		{
			for (int n = 0; n < layer->GetHeight(); n++)
			{
				int tilesetIndex = layer->GetTileTilesetIndex(m, n); // Nghĩa là index của 1 tile ở trong hình tileset sẽ là 1 con số. Khi vào map ta muốn đặt tile đó vào map thì ta lưu con số đó
				if (tilesetIndex != -1) // Do nếu ở map không đặt tile nào thì sẽ đc lưu là -1. Nên ta chỉ xét những số khác -1
				{
					const Tmx::Tileset* tileSet = map->GetTileset(tilesetIndex);

					int tileWidth = map->GetTileWidth(); // 48
					int tileHeight = map->GetTileHeight(); // 48

					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth; // 29
					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight; // 30


					//tile index
					int tileID = layer->GetTileId(m, n); // sẽ có giá trị từ 0

					LPSprite sprite = listTileset[layer->GetTileTilesetIndex(m, n)];

					int y = tileID / tileSetWidth;   
					int x = tileID - y * tileSetWidth; 

					sourceRECT.top = y * tileHeight;
					sourceRECT.bottom = sourceRECT.top + tileHeight;
					sourceRECT.left = x * tileWidth;
					sourceRECT.right = sourceRECT.left + tileWidth;

					D3DXVECTOR2 position(m * tileWidth + tileWidth / 2, n * tileHeight + tileHeight / 2);

					//D3DXVECTOR2 position(m * tileWidth , n * tileHeight );

					//	*************CHECK RECT IN CAMERA*****************
					/*RECT rect;
					rect.left = position.x;
					rect.top = position.y;
					rect.right = rect.left + tileWidth;
					rect.bottom = rect.right + tileHeight;

					if (camera->CheckRectInCamera(rect) == false)
						continue;*/

					// ****************************************************
					count++;

					sprite->SetWidth(tileWidth);
					sprite->SetHeight(tileHeight);

					sprite->SetRect(sourceRECT);

					D3DXVECTOR2 vectorScale = D3DXVECTOR2(1.0f, 1.0f);
					D3DXVECTOR2 newpos = D3DXVECTOR2(position.x + translation.x, position.y + translation.y);

					sprite->Draw(newpos, vectorScale, 0.0f, D3DCOLOR_XRGB(255, 128, 192));

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
