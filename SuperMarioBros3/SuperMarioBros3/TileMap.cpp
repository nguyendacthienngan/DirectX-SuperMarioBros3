#include "TileMap.h"
#include "tinyxml.h"

#include "Ultis.h"
#include "SolidBox.h"
#include "GhostPlatform.h"
#include "MapConst.h"

#include "Koopa.h"
#include "RedKoopa.h"
#include "KoopaShell.h"
#include "Goomba.h"
#include "HitEffects.h"
#include "QuestionBlock.h"

#include "TextureManager.h"
#include "Game.h"

#include <iostream>
#include <map>
#include "Coin.h"

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

TileSet* CTileMap::GetTileSetByTileID(int id)
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
	if (tileSets.size() <= 1)
		return (*tileSets.begin()).second;

	auto iterator = tileSets.lower_bound(id);

	// giá trị trả về của lower_bound functions là 1 bidirectional iterator nên mình có thể --
	if (iterator != tileSets.begin() && (*iterator).first != id) // nếu biến iterator k phải biến đầu tiên và id k giống id đã tìm
		--iterator; // lấy cái trước đó

	return iterator->second; // Bản thân cái map trong c++ tổ chức theo binary tree => nên tìm chỉ tốn O(log2(N))

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

			for (Layer* layer : layers) {
				if (layer->isVisible == false)
					continue;

				// i % width, j % height: Đây là tọa độ của ô 
				int id = layer->tiles[i % width][j % height]; // Từ tọa độ của ô đó ta lấy ra được tileID
				auto tileSet = GetTileSetByTileID(id); // Từ tileID ta tìm tileset mà tileID đó thuộc về
				auto firstGid = tileSet->firstgid;

				if (id >= firstGid)
				{
					auto columns = tileSet->columns;
					auto texture = tileSet->texture;
					auto tileSize = tileSet->tileSize;

					RECT r;
					r.left = ((id - firstGid) % columns) * tileSize.x;
					r.top = ((id - firstGid) / columns) * tileSize.y;
					r.right = r.left + tileSize.x;
					r.bottom = r.top + tileSize.y;
					CGame::GetInstance()->Draw(D3DXVECTOR2(x, y), D3DXVECTOR2(tileSize.x / 2, tileSize.y / 2), texture, r, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
				
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
			TileSet* tileSet = new TileSet();
			element->QueryIntAttribute("firstgid", &tileSet->firstgid);
			element->QueryFloatAttribute("tilewidth", &tileSet->tileSize.x);
			element->QueryFloatAttribute("tileheight", &tileSet->tileSize.y);
			element->QueryIntAttribute("tilecount", &tileSet->tileCount);
			element->QueryIntAttribute("columns", &tileSet->columns);

			TiXmlElement* imgDom = element->FirstChildElement("image");
			string imgPath = imgDom->Attribute("source");
			imgPath = filePath + imgPath;
			CTextureManager::GetInstance()->Add(std::to_string(tileSet->firstgid), imgPath, D3DCOLOR_ARGB(0, 0, 0, 0));
			tileSet->texture = CTextureManager::GetInstance()->GetTexture(std::to_string(tileSet->firstgid));
			gameMap->tileSets[tileSet->firstgid] = tileSet;
		}
		//Load layer
		for (TiXmlElement* element = root->FirstChildElement("layer"); element != nullptr; element = element->NextSiblingElement("layer"))
		{
			Layer* layer = new Layer();
			element->QueryIntAttribute("id", &layer->id);
			element->QueryIntAttribute("width", &layer->width);
			element->QueryIntAttribute("height", &layer->height);

			int visible;
			if (element->QueryIntAttribute("visible", &visible) != TIXML_SUCCESS) layer->isVisible = true;
			else layer->isVisible = visible ? true : false;

			auto tiles = new int* [layer->width]; // số tiles theo chiều ngang

			const char* content = element->FirstChildElement()->GetText();
			vector<string> splitted = split(content, ","); // Trả về chuỗi các tileID trong layer bằng cách tách dấu phẩy ra [ Lúc này chỉ là 1 mảng 1 chiều ]

			for (int i = 0; i < layer->width; i++)
			{
				tiles[i] = new int[layer->height];  // số tiles theo chiều dọc
				for (int j = 0; j < layer->height; j++)
				{
					tiles[i][j] = stoi(splitted[i + j * layer->width]); // stoi giúp chuyển từ string sang int
					// Đem về mảng 2 chiều
				}
			}
			layer->tiles = tiles;
			// Splitted lưu trữ dưới dạng: 0 1 2 3 4 5 6 7 8 9
			// Còn ma trận ta muốn lưu nó sẽ như vầy
			//     0 1 2 3 4 
			//     5 6 7 8 9
			// Ta có i = 0, j = 1
			// Thì muốn lấy 5 ra thì ta phải lấy 0 + 1 * 5 ( i + j * width )
			splitted.clear();
			gameMap->layers.push_back(layer);
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
				else if (name.compare("Ghost") == 0)
				{
					CGhostPlatform* ghostPlatform = new CGhostPlatform();
					ghostPlatform->SetPosition(position - translateConst + size * 0.5);
					ghostPlatform->GetCollisionBox()->at(0)->SetSizeBox(size);
					ghostPlatform->GetCollisionBox()->at(0)->SetId(id);
					ghostPlatform->GetCollisionBox()->at(0)->SetName(nameObject);
					listGameObjects.push_back(ghostPlatform);
					OutputDebugString(ToLPCWSTR("Name object" + nameObject + "\n"));
					DebugOut(L"BoxSize: %d, %f,%f,%f,%f\n", id, ghostPlatform->GetPosition().x, ghostPlatform->GetPosition().y, size.x, size.y);
				}
				else if (name.compare("Enemy") == 0)
				{
					std::string enemyName = object->Attribute("name");
					std::string enemyType = object->Attribute("type");
					if (enemyName.compare("koopa") == 0)
					{
						CKoopaShell* koopaShell = new CKoopaShell();
						koopaShell->SetEnemyType(enemyType);
						koopaShell->SetPosition(position - translateKoopaShellConst);
						koopaShell->SetStartPosition(position - translateKoopaShellConst);

						CRedKoopa* koopa = new CRedKoopa();
						//CKoopa* koopa = new CKoopa();
						koopa->SetEnemyType(enemyType);
						koopa->SetPosition(position - translateKoopaConst);
						koopa->SetStartPosition(position - translateKoopaConst);
						koopa->SetKoopaShell(koopaShell);
						koopaShell->SetKoopa(koopa);
						listGameObjects.push_back(koopaShell);

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
				}
				else if (name.compare("QuestionBlocks") == 0)
				{
					int type;
					std::string name = object->Attribute("name");
					object->QueryIntAttribute("type", &type); //type thiệt ra là số lượng
					CQuestionBlock* solid = new CQuestionBlock();
					solid->SetPosition(position - translateQuestionBlockConst);
					if (name.compare("bcoin") == 0)
					{
						solid->SetItemInfo({ItemTag::Coin, type});
					}
					if (name.compare("sleaf") == 0)
					{
						solid->SetItemInfo({ ItemTag::SuperLeaf, type });

					}
					listGameObjects.push_back(solid);

				}
				else if (name.compare("Coin") == 0)
				{
					CCoin* solid = new CCoin();
					solid->SetPosition(position - translateQuestionBlockConst);
					listGameObjects.push_back(solid);

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
	for (auto tileS : tileSets)
	{
		tileS.second->Clear();
		delete tileS.second;
		tileS.second = NULL;
	}
	tileSets.clear();
	for (auto layer : layers)
	{
		layer->Clear();
		delete layer;
		layer = NULL;
	}
	layers.clear();
}