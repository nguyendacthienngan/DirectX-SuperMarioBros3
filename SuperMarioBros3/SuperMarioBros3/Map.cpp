#include "Map.h"

#include <d3dx9.h>
#include <d3d9.h>

#include "TextureManager.h"
#include "MapConst.h"
#include "Ultis.h"
#include "SolidBox.h"
#include "MarioConst.h"

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

		CTextureManager::GetInstance()->Add(MAP_WORLD1_1_NAME, ToLPCWSTR(MAP_PATH + tileset->GetImage()->GetSource().c_str()), D3DCOLOR_XRGB(255, 128, 192));
		LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->GetTexture(MAP_WORLD1_1_NAME);
		LPSprite sprite = new CSprite(MAP_WORLD1_1_NAME, RECT(), tex);

		listTileset.insert(make_pair(i, sprite)); // insert vào danh sách tileset. Hiện tại mình chỉ có 1 tileset nhưng mình làm tổng quát để sau này có thêm tileset đỡ phải sửa lại

		
		DebugOut(L"[INFO] Loaded Map Successfully \n");
	}


	// Load game objects
	//DebugOut(L"Load game object (1) \n");
	auto objectGroups = map->GetObjectGroups();
	if (objectGroups.size() == 0)
		DebugOut(L"Cannot Load game object (1) \n");

	for (auto objectGroup : objectGroups)
	{
		auto objects = objectGroup->GetObjects();
		auto groupName = objectGroup->GetName();
		//DebugOut(L"Load game object (2) \n");

		if (groupName.compare("Solid") == 0)
		{
			//DebugOut(L"Solid found \n");
			for (int i = 0; i < objects.size(); ++i)
			{
				D3DXVECTOR2 position(objects[i]->GetX(), objects[i]->GetY());
				D3DXVECTOR2 size(objects[i]->GetWidth(), objects[i]->GetHeight());
				int id = objects[i]->GetId();
				string name = std::to_string(id);

				// Tại sao các gameObject trên vật cảnh bị dời theo mario?
				// Big mario sizebox: 42, 81
				// Big marioc rouch sizebox: 42, 42
				// Small mario sizebox: 36, 45
				//position.x += 21; 
				//position.y -= 49; // big mario
				//position.x += 24; 
				//position.y -= 18; //small Mario
				//position.y += MARIO_START_Y;

				//position.x += MARIO_START_X - 42/2 - 4; // 49
				position.x += 45;
				//position.y -= 10;
				position.y -= 10;
				CSolidBox* solid = new CSolidBox();
				solid->SetPosition(position);
				//solid->SetPosition(position + (size * 0.5f)); // tâm giữa
				solid->GetCollisionBox()->at(0)->SetSizeBox(size);
				//solid->GetCollisionBox()->at(0)->SetPosition(D3DXVECTOR2(0.0f, 0.0f));
				//solid->GetCollisionBox()->at(0)->SetPosition(D3DXVECTOR2(MARIO_START_X - 42 / 2 - 4, -10));
				solid->GetCollisionBox()->at(0)->SetName(name);
				solid->GetCollisionBox()->at(0)->SetId(id);
				this->listGameObjects.push_back(solid);
				
				OutputDebugString(ToLPCWSTR("Name object" + name + "\n"));
				DebugOut(L"BoxSize: %d, %f,%f,%f,%f\n", id, solid->GetPosition().x, solid->GetPosition().y, size.x, size.y);
			}
		}
	}
}


void CMap::Update(CCamera* camera, DWORD dt)
{
	if (camera == NULL)
		return;
	camera->Update();
}

// Giải pháp tối ưu: 
// Chỉ loop qua những phần có trong camera
// Chưa làm

void CMap::Render(CCamera* camera)
{
	//DebugOut(L"[INFO] Begin rendering map... \n");

	if (camera == NULL)
	{
		DebugOut(L"[ERROR] Camera is NULL \n");
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
					D3DXVECTOR2 newpos = D3DXVECTOR2(trunc(position.x + translation.x), trunc(position.y + translation.y));

					sprite->Draw(newpos, vectorScale, 0.0f, D3DCOLOR_XRGB(255, 255, 255));

				}
			}
		}
	}
	//DebugOut(L"[INFO] Drawed map sucessfully. \n");
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

std::vector<LPGameObject> CMap::GetListGameObjects()
{
	return listGameObjects;
}
