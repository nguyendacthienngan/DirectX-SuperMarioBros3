#include "TileMap.h"
#include "Ultis.h"
#include "SolidBox.h"
#include "GhostPlatform.h"
#include "MapConst.h"

#include "Koopa.h"
#include "KoopaShell.h"
#include "Goomba.h"

#include <iostream>
#include <map>

CTileMap::CTileMap()
{
	tileWidth = 1;
	tileHeight = 1;
	width = 1;
	height = 1;
}

CTileMap::CTileMap(int width, int height, int tileWidth, int tileHeight)
{
	this->width = width;
	this->height = height;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;
}

LPTileset CTileMap::GetTileSetByTileID(int id)
{
	// Lưu ý là có 2 khái niệm tileid trong file tilemap
	// 1 là cái xét trong tileset: luôn bắt đầu bằng 0  (tileID)
	// 1 là cái xét trong layers: tileId + firstGid của tileset tương ứng (tileID + 1)

	// Tham số truyền vào hàm này chính là tileID TRONG LAYER
	// Do đó khi ta get tileid, ta phải tìm firstgid để biết nó là của tileset nào

	// VD: Ta có 2 tilset, tileset1 có gid là 1, cái thứ 2 có gid là 900
	// Khi lưu vào map, tileid từ 1->899 : thuộc tileset1, 900 trở đi thuộc tileset2

	// Khi lấy tileid trong layer ra để vẽ
	// Trong layer ta có đc id là 950
	// Vậy mình cần tìm cái tileset nào có gid lớn nhất (gần với tileset này nhất) và nhỏ hơn 950 *****
	// 1 nhỏ hơn nhưng nó chưa lớn nhất =>  lấy 900 là gid
	// Vậy tile có tileid = 950 thuộc về tileset2
	
	// Hàm lower_bound của C++ sẽ giúp tìm cái số 900 đó
	if (listTilesets.size() <= 1)
		return (*listTilesets.begin()).second;

	auto iterator = listTilesets.lower_bound (id);

	// giá trị trả về của lower_bound functions là 1 bidirectional iterator nên mình có thể --
	if (iterator != listTilesets.begin() && (*iterator).first != id) // nếu biến iterator k phải biến đầu tiên và id k giống id đã tìm
		--iterator; // lấy cái trước đó
	
	return iterator->second; // Bản thân cái map trong c++ tổ chức theo binary tree => nên tìm chỉ tốn O(log2(N))

}

void CTileMap::AddTileSet(int firstgid, LPTileset tileSet)
{
	this->listTilesets[firstgid] = tileSet;
}

void CTileMap::AddLayer(LPLayer layer)
{
	this->listLayers.push_back(layer);
}

void CTileMap::Render(CCamera* camera)
{
	//DebugOut(L"Render tilemap.. \n");
	// Từ một tọa độ bất kỳ nằm bên trg map
	// Ta sẽ có thể biết được tọa độ nằm trong ô nào của map (Chuyển từ dạng tọa độ về dạng grid)
	// Bằng cách chia cho số tileWidth và tileHeight (Bởi vì 1 tile có size là  tileWidth,tileHeight)

	// Do đó ta sẽ quy tọa đọ của camera về dạng grid
	int col = camera->GetPositionCam().x / tileWidth; 
	int row = camera->GetPositionCam().y / tileHeight;

	// Lấy ra viewport theo dạng grid (số ô)
	D3DXVECTOR2 camSize = D3DXVECTOR2(camera->GetWidthCam() / tileWidth, camera->GetHeightCam() / tileHeight);

	// việc +2 là do trừ hao cho khỏi bị flick ở cạnh màn hình
	// vì sẽ có lúc tính toán làm tròn sao sao đó mà sẽ có ô mình k vẽ
	// do đó mình trừ hao để chắc chắn vẽ hết các ô
	for (int i = col; i < camSize.x + col + 2; i++) 
	{
		for (int j = row; j < camSize.y + row + 2; j++) 
		{

			int x = i * tileWidth - camera->GetPositionCam().x; // vị trí mình muốn vẽ lên màn hình của ô đó => theo tọa độ camera
			int y = j * tileHeight - camera->GetPositionCam().y;

			for (LPLayer layer : listLayers) {
				// i % width, j % height: Đây là tọa độ của ô 
				int id = layer->GetTileID(i % width, j % height); // Từ tọa độ của ô đó ta lấy ra được tileID
				this->GetTileSetByTileID(id)->Draw(id, D3DXVECTOR2(x, y)); // Từ tileID ta tìm tileset mà tileID đó thuộc về
			}
		}
	}
}

CTileMap* CTileMap::LoadMap(std::string filePath, std::string fileMap, std::vector<LPGameObject>& listGameObjects)
{
	string fullPath = filePath + fileMap;
	TiXmlDocument doc(fullPath.c_str());

	if (doc.LoadFile()) {
		OutputDebugString(L"Loading TMX \n");
		TiXmlElement* root = doc.RootElement();
		CTileMap* gameMap = new CTileMap();

		root->QueryIntAttribute("width", &gameMap->width);
		root->QueryIntAttribute("height", &gameMap->height);
		root->QueryIntAttribute("tilewidth", &gameMap->tileWidth);
		root->QueryIntAttribute("tileheight", &gameMap->tileHeight);

		//Load tileset
		for (TiXmlElement* element = root->FirstChildElement("tileset"); element != nullptr; element = element->NextSiblingElement("tileset"))
		{
			CTileset* tileSet = new CTileset(element, filePath);
			gameMap->listTilesets[tileSet->GetFirstgid()] = tileSet;
		}
		//Load layer
		for (TiXmlElement* element = root->FirstChildElement("layer"); element != nullptr; element = element->NextSiblingElement("layer"))
		{
			LPLayer layer = new CLayer(element);
			gameMap->AddLayer(layer);
		}
		// Load game objects
		int count = 0, heightObjectOne = 0;
		for (TiXmlElement* element = root->FirstChildElement("objectgroup"); element != nullptr; element = element->NextSiblingElement("objectgroup"))
		{
			for (TiXmlElement* object = element->FirstChildElement("object"); object != nullptr; object = object->NextSiblingElement("object"))
			{
				std::string name = element->Attribute("name");
				int id, x, y, width, height;
				object->QueryIntAttribute("id", &id);
				object->QueryIntAttribute("x", &x);
				object->QueryIntAttribute("y", &y);
				object->QueryIntAttribute("width", &width);
				object->QueryIntAttribute("height", &height);

				count++;
				if (count == 1)
					heightObjectOne = height;

				D3DXVECTOR2 position = D3DXVECTOR2(x, y);
				D3DXVECTOR2 size = D3DXVECTOR2(width, height);
				string nameObject = std::to_string(id);

				if (name.compare("Solid") == 0)
				{
					CSolidBox* solid = new CSolidBox();
					solid->SetPosition(position - translateConst + size*0.5); // lấy tọa độ giữa
					solid->GetCollisionBox()->at(0)->SetSizeBox(size);
					solid->GetCollisionBox()->at(0)->SetId(id);
					solid->GetCollisionBox()->at(0)->SetName(nameObject);
					listGameObjects.push_back(solid);
					OutputDebugString(ToLPCWSTR("Name object" + nameObject + "\n"));
					DebugOut(L"BoxSize: %d, %f,%f,%f,%f\n", id, solid->GetPosition().x, solid->GetPosition().y, size.x, size.y);
				
				}
				/*else if (name.compare("Ghost") == 0)
				{
					CGhostPlatform* ghostPlatform = new CGhostPlatform();
					ghostPlatform->SetPosition(position);
					ghostPlatform->GetCollisionBox()->at(0)->SetSizeBox(size);
					ghostPlatform->GetCollisionBox()->at(0)->SetId(id);
					ghostPlatform->GetCollisionBox()->at(0)->SetName(nameObject);
					listGameObjects.push_back(ghostPlatform);
				}*/
				else if (name.compare("Enemy") == 0)
				{
					std::string enemyName = object->Attribute("name");
					std::string enemyType = object->Attribute("type");
					if (enemyName.compare("koopa") == 0)
					{
						CKoopa* koopa = new CKoopa();
						koopa->SetEnemyType(enemyType);
						koopa->SetPosition(position - translateKoopaConst);
						koopa->SetStartPosition(position - translateKoopaConst);
						listGameObjects.push_back(koopa);
					}
					else if (enemyName.compare("goomba") == 0)
					{
						CGoomba* goomba = new CGoomba();
						goomba->SetEnemyType(enemyType);
						goomba->SetPosition(position - translateGoombaConst);
						goomba->SetStartPosition(position - translateGoombaConst);
						listGameObjects.push_back(goomba);
					}
					else if (enemyName.compare("koopa-shell") == 0)
					{
						CKoopaShell* koopaShell = new CKoopaShell();
						koopaShell->SetEnemyType(enemyType);
						koopaShell->SetPosition(position - translateKoopaShellConst);
						koopaShell->SetStartPosition(position - translateKoopaShellConst);
						listGameObjects.push_back(koopaShell);
					}
				}
			}
		}
		if (listGameObjects.size() == 0)
			DebugOut(L"[ERROR] Cannot load game objects \n");
		return gameMap;
	}
	DebugOut(L"[ERROR] Cannnot load file map \n");
	return nullptr;
}

CTileMap::~CTileMap()
{
}