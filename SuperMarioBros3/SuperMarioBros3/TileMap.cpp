#include "TileMap.h"

#include "MapConst.h"
#include "Grid.h"

#include <iostream>
#include <map>
#include "Game.h"
#include "Ultis.h"
#include "TextureManager.h"

#include "SolidBox.h"
#include "GhostPlatform.h"

#include "GreenKoopa.h"
#include "RedKoopa.h"
#include "RedKoopaShell.h"
#include "GreenKoopaShell.h"

#include "GreenParaKoopa.h"
#include "RedParaKoopa.h"

#include "Goomba.h"
#include "RedParaGoomba.h"
#include "RedGoomba.h"
#include "TanGoomba.h"

#include "HitEffects.h"
#include "QuestionBlock.h"
#include "Coin.h"
#include "Brick.h"
#include "Piranha.h"

#include "VenusGreen.h"
#include "VenusRed.h"

#include "Portal.h"
#include "Label.h"
#include "Grass.h"

#include "HelpItem.h"
#include "StartItem.h"
#include "SceneGate.h"
#include "WorldItemConst.h"
#include "NodeMap.h"
#include "PSwitch.h"
#include "EmptyBlock.h"
#include "Card.h"
#include "MovingPlatform.h"
#include "ThreeAnimation.h"
using namespace std;
CTileMap::CTileMap()
{
	tileWidth = 1;
	tileHeight = 1;
	width = 1;
	height = 1;
	foreground = NULL;
	poolBricks = new CObjectPool();
	poolCoins = new CObjectPool();
	card = NULL;
	grid = NULL;
}

CTileMap::CTileMap(int width, int height, int tileWidth, int tileHeight)
{
	this->width = width;
	this->height = height;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;
	foreground = NULL;
	poolBricks = new CObjectPool();
	poolCoins = new CObjectPool();
	card = NULL;
	player = NULL;
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

void CTileMap::Render(CCamera* camera, bool isRenderForeground)
{

	//DebugOut(L"Render tilemap.. \n");
	// Từ một tọa độ bất kỳ nằm bên trg map
	// Ta sẽ có thể biết được tọa độ nằm trong ô nào của map (Chuyển từ dạng tọa độ về dạng grid)
	// Bằng cách chia cho số tileWidth và tileHeight (Bởi vì 1 tile có size là  tileWidth,tileHeight)

	// Do đó ta sẽ quy tọa đọ của camera về dạng grid
	int col = abs(camera->GetPositionCam().x / tileWidth);
	int row = abs(camera->GetPositionCam().y / tileHeight);

	// Lấy ra viewport theo dạng grid (số ô)
	D3DXVECTOR2 camSize = D3DXVECTOR2(camera->GetWidthCam() / tileWidth, camera->GetHeightCam() / tileHeight);

	// việc +2 là do trừ hao cho khỏi bị flick ở cạnh màn hình
	// vì sẽ có lúc tính toán làm tròn sao sao đó mà sẽ có ô mình k vẽ
	// do đó mình trừ hao để chắc chắn vẽ hết các ô
	for (int i = col; i < camSize.x + col + 4; i++)
	{
		for (int j = row; j < camSize.y + row + 4; j++)
		{
			int x = i * tileWidth - camera->GetPositionCam().x; // vị trí mình muốn vẽ lên màn hình của ô đó => theo tọa độ camera
			int y = j * tileHeight - camera->GetPositionCam().y;
			if (isRenderForeground == true)
			{
				if (foreground == NULL)
					continue;
				RenderLayer(foreground, i, j, x, y);
			}
			else
			{
				for (Layer* layer : layers)
				{
					RenderLayer(layer, i, j, x, y);
				}
			}

		}
	}

}

CTileMap* CTileMap::LoadMap(std::string filePath, std::string fileMap, std::vector<LPGameObject>& listGameObjects, CGameObject* player, CScene* scene)
{
	string fullPath = filePath + fileMap;
	TiXmlDocument doc(fullPath.c_str());
	if (doc.LoadFile())
	{
		OutputDebugString(L"Loading TMX \n");
		TiXmlElement* root = doc.RootElement();
		graph = new CGraph();

		root->QueryIntAttribute("width", &width);
		root->QueryIntAttribute("height", &height);
		root->QueryIntAttribute("tilewidth", &tileWidth);
		root->QueryIntAttribute("tileheight", &tileHeight);

		this->grid = new CGrid(D3DXVECTOR2(width * tileWidth, height * tileHeight));
		this->player = player;
		this->scene = scene;
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
			tileSet->textureID = std::to_string(tileSet->firstgid);
			CTextureManager::GetInstance()->Add(std::to_string(tileSet->firstgid), imgPath, D3DCOLOR_ARGB(0, 0, 0, 0));
			tileSet->texture = CTextureManager::GetInstance()->GetTexture(std::to_string(tileSet->firstgid));
			tileSets[tileSet->firstgid] = tileSet;
		}
		//Load layer
		for (TiXmlElement* element = root->FirstChildElement("layer"); element != nullptr; element = element->NextSiblingElement("layer"))
		{
			std::string name = element->Attribute("name");
			auto layer = LoadLayer(element);
			if (layer != NULL)
			{
				if (name.compare("Foreground") == 0)
					foreground = layer;
				else
					layers.push_back(layer);
			}

		}
		// Load game objects
		//std::unordered_map<int, CGameObject*> objectGroup;
		for (TiXmlElement* element = root->FirstChildElement("objectgroup"); element != nullptr; element = element->NextSiblingElement("objectgroup"))
		{
			std::string name = element->Attribute("name");
			int objectGroupdID;
			element->QueryIntAttribute("id", &objectGroupdID);

			/*if (objectGroup.size() > 0)
				objectGroups.insert(make_pair(objectGroupdID, objectGroup));
			objectGroup.clear();*/

			CGameObject* gameObject = NULL;
			for (TiXmlElement* object = element->FirstChildElement("object"); object != nullptr; object = object->NextSiblingElement("object"))
			{
				int id, x, y, width, height;
				int type = 0;
				int cellX = -1, cellY = -1;
				object->QueryIntAttribute("id", &id);
				object->QueryIntAttribute("x", &x);
				object->QueryIntAttribute("y", &y);
				object->QueryIntAttribute("width", &width);
				object->QueryIntAttribute("height", &height);


				D3DXVECTOR2 position = D3DXVECTOR2(x, y);
				D3DXVECTOR2 size = D3DXVECTOR2(width, height);
				string nameObject = std::to_string(id);

				TiXmlElement* properties = object->FirstChildElement();
				if (properties != NULL)
				{
					for (TiXmlElement* property = properties->FirstChildElement(); property != NULL; property = property->NextSiblingElement())
					{
						std::string propName = property->Attribute("name");
						if (propName.compare("cellx") == 0)
						{
							property->QueryIntAttribute("value", &cellX);
						}
						if (propName.compare("celly") == 0)
						{
							property->QueryIntAttribute("value", &cellY);
						}
					}
				}

				if (name.compare("Solid") == 0)
				{
					gameObject = LoadSolidBox(position, size, nameObject, listGameObjects);
				}
				else if (name.compare("Ghost") == 0)
				{
					gameObject = LoadGhostBox(position, size, nameObject, listGameObjects);
				}
				else if (name.compare("Enemy") == 0)
				{
					std::string enemyName = object->Attribute("name");
					std::string enemyType = object->Attribute("type");
					gameObject = LoadEnemy(position, enemyName, enemyType, object, listGameObjects);
				}
				else if (name.compare("QuestionBlocks") == 0)
				{
					object->QueryIntAttribute("type", &type); //type thiệt ra là số lượng
					std::string questionBlockName = object->Attribute("name");
					gameObject = LoadQuestionBlock(position, type, questionBlockName, listGameObjects);
				}
				else if (name.compare("Coin") == 0)
				{
					gameObject = LoadCoin(position, type, object, listGameObjects);
				}
				else if (name.compare("Brick") == 0)
				{
					gameObject = LoadBrick(position, type, object, listGameObjects);
				}
				else if (name.compare("Portal") == 0)
				{
					gameObject = LoadPortal(position, size, object, listGameObjects);
				}
				else if (name.compare("Label") == 0)
				{
					std::string labelName = object->Attribute("name");
					gameObject = LoadLabel(position, labelName, size, object, listGameObjects);
				}
				else if (name.compare("World-Item") == 0)
				{
					string itemName = object->Attribute("name");
					gameObject = LoadWorldItem(position, itemName, listGameObjects);
				}
				else if (name.compare("Portal-Scene") == 0)
				{
					gameObject = LoadPortalScene(position, size, object, listGameObjects);
				}
				else if (name.compare("SwitchBlocks") == 0)
				{
					gameObject = LoadSwitchBlock(position, listGameObjects);
				}
				else if (name.compare("Block") == 0)
				{
					gameObject = LoadEmptyBlock(position, listGameObjects);
				}
				else if (name.compare("Card") == 0)
				{
					gameObject = LoadCard(position, listGameObjects);
				}
				else if (name.compare("MovingPlatform") == 0)
				{
					gameObject = LoadMovingPlatform(position, listGameObjects);
				}

				if (cellX != -1 && cellY != -1)
					gameObject->SetIndex({ cellX, cellY });
				/*if (gameObject != NULL)
				{
					objectGroup.insert(make_pair(id, gameObject));
				}*/
			}
		}
		if (listGameObjects.size() == 0 && scene->IsSpacePartitioning() == false)
		{
			DebugOut(L"[ERROR] Cannot load game objects \n");
			return nullptr;
		}
		return this;
	}
	DebugOut(L"[ERROR] Cannnot load file map \n");
	return nullptr;
}

Layer* CTileMap::LoadLayer(TiXmlElement* element)
{
	Layer* layer = new Layer();

	int visible;
	if (element->QueryIntAttribute("visible", &visible) != TIXML_SUCCESS) layer->isVisible = true;
	else layer->isVisible = visible;
	if (visible == 0)
	{
		delete layer;
		layer = NULL;
		return NULL;
	}
	element->QueryIntAttribute("id", &layer->id);
	element->QueryIntAttribute("width", &layer->width);
	element->QueryIntAttribute("height", &layer->height);

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
	return layer;
}

CGameObject* CTileMap::LoadSolidBox(D3DXVECTOR2 position, D3DXVECTOR2 size, std::string name, std::vector<LPGameObject>& listGameObjects)
{
	CSolidBox* solid = new CSolidBox();
	solid->SetPosition(position - translateConst + size * 0.5); // lấy tọa độ giữa
	solid->GetCollisionBox()->at(0)->SetSizeBox(size);
	solid->GetCollisionBox()->at(0)->SetName(name);
	AddObjectToList(solid, listGameObjects);
	return solid;
}

CGameObject* CTileMap::LoadGhostBox(D3DXVECTOR2 position, D3DXVECTOR2 size, std::string name, std::vector<LPGameObject>& listGameObjects)
{
	CGhostPlatform* ghostPlatform = new CGhostPlatform();
	ghostPlatform->SetPosition(position - translateConst + size * 0.5);
	ghostPlatform->GetCollisionBox()->at(0)->SetSizeBox(size);
	ghostPlatform->GetCollisionBox()->at(0)->SetName(name);
	AddObjectToList(ghostPlatform, listGameObjects);
	return ghostPlatform;
}

CGameObject* CTileMap::LoadEnemy(D3DXVECTOR2 position, std::string enemyName, std::string enemyType, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects)
{
	CGameObject* enemy = NULL;
	if (enemyName.compare("koopa") == 0)
	{
		enemy = LoadKoopa(position, enemyType, listGameObjects);
	}
	else if (enemyName.compare("goomba") == 0)
	{
		enemy = LoadGoomba(position, enemyType, listGameObjects);
	}
	else if (enemyName.compare("para-goomba") == 0)
	{
		enemy = LoadParagoomba(position, enemyType, listGameObjects);
	}
	else if (enemyName.compare("para-koopa") == 0)
	{
		enemy = LoadParakoopa(position, enemyType, object, listGameObjects);
	}
	else if (enemyName.compare("piranha") == 0)
	{
		enemy = LoadPiranha(position, listGameObjects);
	}
	else if (enemyName.compare("venus") == 0)
	{
		enemy = LoadVenus(position, enemyType, listGameObjects);
	}
	return enemy;
}

CGameObject* CTileMap::LoadKoopa(D3DXVECTOR2 position, std::string enemyType, std::vector<LPGameObject>& listGameObjects)
{
	CKoopa* koopa = NULL;
	CKoopaShell* koopaShell = NULL;
	if (enemyType.compare("green") == 0)
	{
		koopaShell = new CGreenKoopaShell();
		koopa = new CGreenKoopa();
		koopa->Enable(true);
	}
	if (enemyType.compare("red") == 0)
	{
		koopaShell = new CRedKoopaShell();
		koopa = new CRedKoopa();
	}
	if (koopa != NULL && koopaShell != NULL)
	{
		koopaShell->SetEnemyType(enemyType);
		koopaShell->SetPosition(position - translateConst);
		koopaShell->SetStartPosition(position - translateConst);

		koopa->SetEnemyType(enemyType);
		koopa->SetPosition(position - translateConst);
		koopa->SetStartPosition(position - translateConst);
		koopa->SetKoopaShell(koopaShell);

		koopaShell->SetKoopa(koopa);
		koopaShell->SetTarget(player);
		koopaShell->Enable(false);
		koopa->SetTarget(player);

		AddObjectToList(koopaShell, listGameObjects);
		AddObjectToList(koopa, listGameObjects);
	}
	return koopa;
}

CGameObject* CTileMap::LoadParakoopa(D3DXVECTOR2 position, std::string enemyType, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects)
{

	if (enemyType.compare("red") == 0)
	{
		int boundaryTop = 0, boundaryBottom = 0;
		TiXmlElement* properties = object->FirstChildElement();
		for (TiXmlElement* property = properties->FirstChildElement(); property != NULL; property = property->NextSiblingElement())
		{
			std::string propName = property->Attribute("name");
			if (propName.compare("top") == 0)
			{
				property->QueryIntAttribute("value", &boundaryTop);
			}
			if (propName.compare("bottom") == 0)
			{
				property->QueryIntAttribute("value", &boundaryBottom);
			}
		}
		if (boundaryTop == 0 || boundaryBottom == 0) return NULL;

		CRedKoopaShell* koopaShell = new CRedKoopaShell();
		koopaShell->SetEnemyType(enemyType);
		koopaShell->SetPosition(position - translateConst);
		koopaShell->SetStartPosition(position - translateConst);
		koopaShell->Enable(false);
		koopaShell->SetTarget(player);

		CRedKoopa* koopa = new CRedKoopa();
		koopa->SetEnemyType(enemyType);
		koopa->SetPosition(position - translateConst);
		koopa->SetStartPosition(position - translateConst);
		koopa->SetKoopaShell(koopaShell);
		koopaShell->SetKoopa(koopa);
		koopa->Enable(false);
		koopa->SetTarget(player);

		CRedParaKoopa* parakoopa = new CRedParaKoopa();
		auto pos = position - translateConst;
		parakoopa->SetPosition(pos);
		parakoopa->SetStartPosition(pos);
		parakoopa->SetKoopa(koopa);
		parakoopa->SetBoundary(pos.y - boundaryTop, pos.y + boundaryBottom);
		parakoopa->SetTarget(player);

		AddObjectToList(koopaShell, listGameObjects);
		AddObjectToList(koopa, listGameObjects);
		AddObjectToList(parakoopa, listGameObjects);
		return parakoopa;
	}
	else
	{
		CGreenKoopaShell* koopaShell = new CGreenKoopaShell();
		koopaShell->SetEnemyType(enemyType);
		koopaShell->SetPosition(position - translateConst);
		koopaShell->SetStartPosition(position - translateConst);
		koopaShell->Enable(false);
		koopaShell->SetTarget(player);

		CGreenKoopa* koopa = new CGreenKoopa();
		koopa->SetEnemyType(enemyType);
		koopa->SetPosition(position - translateConst);
		koopa->SetStartPosition(position - translateConst);
		koopa->SetKoopaShell(koopaShell);
		koopaShell->SetKoopa(koopa);
		koopa->Enable(false);
		koopa->SetTarget(player);

		CGreenParaKoopa* parakoopa = new CGreenParaKoopa();
		parakoopa->SetPosition(position - translateConst);
		parakoopa->SetStartPosition(position - translateConst);
		parakoopa->SetKoopa(koopa);
		parakoopa->SetTarget(player);

		AddObjectToList(koopaShell, listGameObjects);
		AddObjectToList(koopa, listGameObjects);
		AddObjectToList(parakoopa, listGameObjects);
		return parakoopa;
	}
	return NULL;
}

CGameObject* CTileMap::LoadGoomba(D3DXVECTOR2 position, std::string enemyType, std::vector<LPGameObject>& listGameObjects)
{
	CGoomba* goomba = NULL;
	if (enemyType.compare("tan") == 0)
	{
		goomba = new CTanGoomba();
	}
	if (enemyType.compare("red") == 0)
	{
		goomba = new CRedGoomba();
	}
	if (goomba != NULL)
	{
		goomba->SetEnemyType(enemyType);
		goomba->SetPosition(position - translateConst);
		goomba->SetStartPosition(position - translateConst);
		goomba->SetTarget(player);
		AddObjectToList(goomba, listGameObjects);
	}
	return goomba;
}

CGameObject* CTileMap::LoadParagoomba(D3DXVECTOR2 position, std::string enemyType, std::vector<LPGameObject>& listGameObjects)
{
	CRedGoomba* goomba = new CRedGoomba();
	goomba->SetPosition(position - translateConst);
	goomba->SetStartPosition(position - translateConst);
	goomba->Enable(false);

	CRedParaGoomba* paragoomba = new CRedParaGoomba();
	paragoomba->SetPosition(position - translateConst);
	paragoomba->SetStartPosition(position - translateConst);
	paragoomba->SetGoomba(goomba);
	paragoomba->SetTarget(player);
	AddObjectToList(goomba, listGameObjects);
	AddObjectToList(paragoomba, listGameObjects);
	return paragoomba;
}

CGameObject* CTileMap::LoadPiranha(D3DXVECTOR2 position, std::vector<LPGameObject>& listGameObjects)
{
	CPiranha* piranha = new CPiranha();
	piranha->SetPosition(position - translateConst);
	piranha->SetStartPosition(position - translateConst);
	piranha->SetTarget(player);
	AddObjectToList(piranha, listGameObjects);
	return piranha;
}

CGameObject* CTileMap::LoadVenus(D3DXVECTOR2 position, std::string enemyType, std::vector<LPGameObject>& listGameObjects)
{
	CVenus* venus = NULL;
	if (enemyType.compare("green") == 0)
		venus = new CVenusGreen();
	else
		venus = new CVenusRed();
	venus->SetPosition(position - translateConst);
	venus->SetStartPosition(position - translateConst);
	venus->SetTarget(player);
	venus->GetObjectPool().AddPoolToScene(scene);
	AddObjectToList(venus, listGameObjects);
	return venus;
}

CGameObject* CTileMap::LoadQuestionBlock(D3DXVECTOR2 position, int type, std::string name, std::vector<LPGameObject>& listGameObjects)
{
	CQuestionBlock* solid = new CQuestionBlock();
	solid->SetPosition(position - translateConst);
	if (name.compare("bcoin") == 0)
	{
		solid->SetItemInfo({ ItemTag::Coin, type });
	}
	if (name.compare("powerup") == 0)
	{
		solid->SetItemInfo({ ItemTag::PowerUp, type });
	}

	solid->SetTarget(player);
	AddObjectToList(solid, listGameObjects);
	return solid;
}

CGameObject* CTileMap::LoadBrick(D3DXVECTOR2 position, int type, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects)
{
	CBrick* solid = new CBrick();
	solid->SetPosition(position - translateConst);
	if (object->QueryIntAttribute("type", &type) == TIXML_SUCCESS && type == 1)
	{
		bricks.push_back(solid);
		CCoin* coin = new CCoin();
		coin->SetType(type);

		poolCoins->Add(coin);
		poolBricks->Add(solid);

		solid->Enable(true);
	}
	solid->SetType(type);
	solid->GetObjectPool().AddPoolToScene(scene);
	AddObjectToList(solid, listGameObjects);
	return solid;
}

CGameObject* CTileMap::LoadCoin(D3DXVECTOR2 position, int type, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects)
{
	CCoin* solid = new CCoin();
	solid->SetPosition(position - translateConst);
	if (object->QueryIntAttribute("type", &type) == TIXML_SUCCESS && type == 1)
	{
		coins.push_back(solid);
		CBrick* brick = new CBrick();
		brick->SetType(type);

		poolBricks->Add(brick);
		poolCoins->Add(solid);

		solid->Enable(true);
	}
	solid->SetType(type);
	AddObjectToList(solid, listGameObjects);
	return solid;
}

CGameObject* CTileMap::LoadPortal(D3DXVECTOR2 position, D3DXVECTOR2 size, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects)
{
	int cameraID = -1;
	std::string sceneID = "";

	CPortal* portal = new CPortal(size);
	portal->SetPosition(position - translateConst + size * 0.5);

	TiXmlElement* properties = object->FirstChildElement();
	for (TiXmlElement* property = properties->FirstChildElement(); property != NULL; property = property->NextSiblingElement())
	{
		std::string propName = property->Attribute("name");
		if (propName.compare("cameraID") == 0)
		{
			property->QueryIntAttribute("value", &cameraID);
			portal->SetCameraID(cameraID);
		}
		if (propName.compare("sceneID") == 0)
		{
			sceneID = property->Attribute("value");
			portal->SetSceneID(sceneID);
		}
	}
	AddObjectToList(portal, listGameObjects);
	return portal;

}

CGameObject* CTileMap::LoadLabel(D3DXVECTOR2 position, std::string labelName, D3DXVECTOR2 size, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects)
{
	if (labelName.compare("warp-pipe") == 0)
	{
		CLabel* label = new CLabel(size);
		label->SetPosition(position - translateConst + size * 0.5);

		TiXmlElement* properties = object->FirstChildElement();
		for (TiXmlElement* property = properties->FirstChildElement(); property != NULL; property = property->NextSiblingElement())
		{
			std::string propName = property->Attribute("name");
			if (propName.compare("direction") == 0)
			{
				std::string direction = property->Attribute("value");
				if (direction.compare("up") == 0)
					label->SetPushDirection({ 0, 1, 0, 0 });
				if (direction.compare("down") == 0)
					label->SetPushDirection({ 0, 0, 0, 1 });
				if (direction.compare("right") == 0)
					label->SetPushDirection({ 0, 0, 1, 0 });
				if (direction.compare("left") == 0)
					label->SetPushDirection({ 1, 0, 0, 0 });
			}
		}
		AddObjectToList(label, listGameObjects);
		return label;
	}
	return NULL;
}

CGameObject* CTileMap::LoadWorldItem(D3DXVECTOR2 position, std::string itemName, std::vector<LPGameObject>& listGameObjects)
{
	if (itemName.compare("grass") == 0)
	{
		CGrass* grass = new CGrass();
		grass->SetPosition(position - translateConst);
		AddObjectToList(grass, listGameObjects);
		return grass;
	}
	if (itemName.compare("help") == 0)
	{
		CHelpItem* help = new CHelpItem();
		help->SetPosition(position - translateConst);
		AddObjectToList(help, listGameObjects);
		return help;
	}
	if (itemName.compare("start") == 0)
	{
		CStartItem* startItem = new CStartItem();
		startItem->SetPosition(position - translateConst);
		AddObjectToList(startItem, listGameObjects);
		return startItem;
	}
	if (itemName.compare("three") == 0)
	{
		CThreeAnimation* three = new CThreeAnimation();
		three->SetPosition(position - translateConst);
		AddObjectToList(three, listGameObjects);
		return three;
	}
	return NULL;
}

CGameObject* CTileMap::LoadPortalScene(D3DXVECTOR2 position, D3DXVECTOR2 size, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects)
{
	std::string type = object->Attribute("type");
	CGameObject* portal = NULL;
	if (type.compare("scene") == 0)
	{
		portal = LoadSceneGate(position, size, object, listGameObjects);
	}
	if (type.compare("node") == 0)
	{
		portal = LoadNodeGate(position, size, object, listGameObjects);
	}
	return portal;
}

CGameObject* CTileMap::LoadSceneGate(D3DXVECTOR2 position, D3DXVECTOR2 size, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects)
{
	int cameraID = -1;
	std::string sceneID = "";
	std::string sceneName = object->Attribute("name");
	CSceneGate* portal = new CSceneGate(size);
	portal->SetPosition(position - translateConst + size * 0.5);
	if (sceneName.compare("scene-1") == 0)
	{
		portal->SetState(SCENE_1_ANIMATION);
		portal->AddAdjacencyNode(1);
		portal->AddAdjacencyNode(3);
	}
	if (sceneName.compare("scene-2") == 0)
	{
		portal->SetState(SCENE_2_ANIMATION);
		portal->AddAdjacencyNode(3);
		portal->AddAdjacencyNode(5);
		portal->AddAdjacencyNode(9);
	}
	if (sceneName.compare("scene-3") == 0)
	{
		portal->SetState(SCENE_3_ANIMATION);
		portal->AddAdjacencyNode(4);
		portal->AddAdjacencyNode(6);
	}
	if (sceneName.compare("scene-4") == 0)
	{
		portal->SetState(SCENE_4_ANIMATION);
		portal->AddAdjacencyNode(7);
		portal->AddAdjacencyNode(9);

	}
	if (sceneName.compare("scene-5") == 0)
	{
		portal->SetState(SCENE_5_ANIMATION);
		portal->AddAdjacencyNode(14);
		portal->AddAdjacencyNode(16);

	}
	if (sceneName.compare("scene-6") == 0)
	{
		portal->SetState(SCENE_6_ANIMATION);
		portal->AddAdjacencyNode(16);
		portal->AddAdjacencyNode(18);

	}
	if (sceneName.compare("spade") == 0)
	{
		portal->SetState(SPADE_ANIMATION);
		portal->AddAdjacencyNode(9);
		portal->AddAdjacencyNode(11);

	}
	if (sceneName.compare("castle") == 0)
	{
		portal->SetState(CASTLE_ANIMATION);
		portal->AddAdjacencyNode(10);
		portal->AddAdjacencyNode(12);

	}
	if (sceneName.compare("domed-gate") == 0)
	{
		portal->SetState(DOMED_ANIMATION);
		portal->AddAdjacencyNode(1);
		portal->AddAdjacencyNode(12);

	}
	if (sceneName.compare("mushroom-gate-1") == 0)
	{
		portal->SetState(MUSHROOM_ANIMATION);
		portal->AddAdjacencyNode(6);
		portal->AddAdjacencyNode(8);

	}
	if (sceneName.compare("mushroom-gate-2") == 0)
	{
		portal->SetState(MUSHROOM_ANIMATION);
		portal->AddAdjacencyNode(18);
	}
	TiXmlElement* properties = object->FirstChildElement();
	if (properties != NULL)
	{
		for (TiXmlElement* property = properties->FirstChildElement(); property != NULL; property = property->NextSiblingElement())
		{
			std::string propName = property->Attribute("name");
			if (propName.compare("cameraID") == 0)
			{
				property->QueryIntAttribute("value", &cameraID);
				portal->SetCameraID(cameraID);
			}
			if (propName.compare("sceneID") == 0)
			{
				sceneID = property->Attribute("value");
				portal->SetSceneID(sceneID);
			}
			if (propName.compare("nodeID") == 0)
			{
				int nodeID;
				property->QueryIntAttribute("value", &nodeID);
				portal->SetNodeID(nodeID);
			}
		}
		if (portal != NULL)
		{
			graph->AddNode(portal);
			AddObjectToList(portal, listGameObjects);
			return portal;
		}
	}
	return NULL;
}

CGameObject* CTileMap::LoadNodeGate(D3DXVECTOR2 position, D3DXVECTOR2 size, TiXmlElement* object, std::vector<LPGameObject>& listGameObjects)
{
	CNodeMap* node = new CNodeMap(size);

	node->SetPosition(position - translateConst + size * 0.5);
	TiXmlElement* properties = object->FirstChildElement();
	if (properties != NULL)
	{
		for (TiXmlElement* property = properties->FirstChildElement(); property != NULL; property = property->NextSiblingElement())
		{
			std::string propName = property->Attribute("name");
			if (propName.compare("nodeID") == 0)
			{
				int nodeID;
				property->QueryIntAttribute("value", &nodeID);
				node->SetNodeID(nodeID);
				if (nodeID == 0)
				{
					node->AddAdjacencyNode(1);
				}
				if (nodeID == 1)
				{
					node->AddAdjacencyNode(0);
					node->AddAdjacencyNode(2);
				}
				if (nodeID == 3)
				{
					node->AddAdjacencyNode(2);
					node->AddAdjacencyNode(4);
				}
				if (nodeID == 6)
				{
					node->AddAdjacencyNode(5);
					node->AddAdjacencyNode(7);
				}
				if (nodeID == 9)
				{
					node->AddAdjacencyNode(8);
					node->AddAdjacencyNode(10);
					node->AddAdjacencyNode(4);
				}
				if (nodeID == 12)
				{
					node->AddAdjacencyNode(11);
					node->AddAdjacencyNode(13);
					node->AddAdjacencyNode(14);
				}
				if (nodeID == 14)
				{
					node->AddAdjacencyNode(12);
					node->AddAdjacencyNode(15);
				}
				if (nodeID == 16)
				{
					node->AddAdjacencyNode(15);
					node->AddAdjacencyNode(17);
				}
				if (nodeID == 18)
				{
					node->AddAdjacencyNode(17);
					node->AddAdjacencyNode(19);
					node->AddAdjacencyNode(20);
				}
				if (nodeID == 20)
				{
					node->AddAdjacencyNode(18);
					node->AddAdjacencyNode(21);
				}
				if (nodeID == 21)
				{
					node->AddAdjacencyNode(10);
				}
			}
		}
	}
	if (node != NULL)
	{
		graph->AddNode(node);
		AddObjectToList(node, listGameObjects);
		return node;
	}
	return NULL;
}

CGameObject* CTileMap::LoadSwitchBlock(D3DXVECTOR2 position, std::vector<LPGameObject>& listGameObjects)
{
	CPSwitch* switchBlock = new CPSwitch();
	switchBlock->SetPosition(position - translateConst);
	AddObjectToList(switchBlock, listGameObjects);
	return switchBlock;
}

CGameObject* CTileMap::LoadEmptyBlock(D3DXVECTOR2 position, std::vector<LPGameObject>& listGameObjects)
{
	CEmptyBlock* emptyBlock = new CEmptyBlock();
	emptyBlock->SetPosition(position - translateConst);
	AddObjectToList(emptyBlock, listGameObjects);
	return emptyBlock;
}

CGameObject* CTileMap::LoadCard(D3DXVECTOR2 position, std::vector<LPGameObject>& listGameObjects)
{
	CCard* ca = new CCard();
	ca->SetPosition(position - translateConst);
	AddObjectToList(ca, listGameObjects);
	card = ca;
	return ca;
}

CGameObject* CTileMap::LoadMovingPlatform(D3DXVECTOR2 position, std::vector<LPGameObject>& listGameObjects)
{
	CMovingPlatform* platform = new CMovingPlatform();
	platform->SetPosition(position);
	AddObjectToList(platform, listGameObjects);
	return platform;
}

void CTileMap::RenderLayer(Layer* layer, int i, int j, int x, int y)
{
	if (layer->isVisible == false || layer == NULL)
		return;
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

CGraph* CTileMap::GetGraph()
{
	return graph;
}

std::vector<CGameObject*> CTileMap::GetBricks()
{
	return bricks;
}

std::vector<CGameObject*> CTileMap::GetCoins()
{
	return coins;
}

CObjectPool* CTileMap::GetPoolBricks()
{
	return poolBricks;
}

CObjectPool* CTileMap::GetPoolCoins()
{
	return poolCoins;
}

CGameObject* CTileMap::GetCard()
{
	return card;
}

void CTileMap::AddObjectToList(CGameObject* gO)
{
	grid->Insert(gO);
}

void CTileMap::AddObjectToList(CGameObject* gameObject, std::vector<LPGameObject>& gameObjects)
{
	if (gameObject == NULL)
		return;

	if (scene->IsSpacePartitioning() == true)
	{
		if (scene->CheckGlobalObject(gameObject->GetTag()))
			scene->AddGlobalObject(gameObject);
		else
		{
			if (gameObject->IsInGrid() == false)
			{
				grid->Insert(gameObject);
				gameObject->SetInGrid(true);
			}
		}
	}
	else
	{
		auto gameObj = find(gameObjects.begin(), gameObjects.end(), gameObject);
		if (gameObj == gameObjects.end())
		{
			gameObjects.push_back(gameObject);
		}
	}
}

CGrid* CTileMap::GetGrid()
{
	return grid;
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
	delete foreground;
}