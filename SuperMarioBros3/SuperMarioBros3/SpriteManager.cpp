#include "SpriteManager.h"
#include "Game.h"
#include "Const.h"
#include "GraphicConst.h"
#include "Ultis.h"
#include "tinyxml.h"
#include "TextureManager.h"
using namespace std;

LPSpriteManager CSpriteManager::instance = NULL;

CSpriteManager::CSpriteManager()
{
}

void CSpriteManager::Init()
{
	auto root = CGame::GetInstance();
	LoadSprite(root->GetFilePathByCategory(CATEGORY_SPRITE, DB_SPRITE_MARIO));
}

void CSpriteManager::Add(string id, RECT rect, LPDIRECT3DTEXTURE9 tex, int xPivot)
{
	LPSprite s = new CSprite(id, xPivot, rect, tex);
	sprites.insert(make_pair(id, s));
}

bool CSpriteManager::LoadSprite(string filePath)
{
	DebugOut(L"[INFO] Load Sprite From XML \n");
	OutputDebugStringW(ToLPCWSTR(filePath.c_str()));
	DebugOut(L"\n");
	TiXmlDocument document(filePath.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();
	TiXmlElement* texture = root->FirstChildElement();

	string textureID = texture->Attribute("id");
	LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->GetTexture(textureID);

	if (tex != nullptr)
		OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));

	for (TiXmlElement* node = texture->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
	{

		string spriteID = node->Attribute("id");
		int left, top, width, height, pivotX;
		node->QueryIntAttribute("left", &left);
		node->QueryIntAttribute("top", &top);
		node->QueryIntAttribute("width", &width);
		node->QueryIntAttribute("height", &height);

		if (node->QueryIntAttribute("xPivot", &pivotX) != TIXML_SUCCESS)
		{
			pivotX = -1;
		}

		pivotX *= 3;
		OutputDebugStringW(ToLPCWSTR(spriteID + ':' + to_string(left) + ':' + to_string(top) + ':' + to_string(width) + ':' + to_string(height) + ':' + to_string(pivotX) + '\n'));
		
		RECT rect;
		rect.left = left*3;
		rect.top = top * 3;
		rect.right = (left + width)*3;
		rect.bottom = (top + height)*3;

		Add(spriteID, rect, tex, pivotX);
	}
	// Load Sprite from XML
	return true;
}

LPSprite CSpriteManager::Get(string id)
{
	return sprites[id];
}

void CSpriteManager::Clear()
{
	for (auto x : sprites)
	{
		LPSprite s = x.second;
		delete s;
	}

	sprites.clear();
}

LPSpriteManager CSpriteManager::GetInstance()
{
	if (instance == NULL) instance = new CSpriteManager();
	return instance;
}

CSpriteManager::~CSpriteManager()
{
	/*for (auto s : sprites)
		delete s.second;*/
}