#include "SpriteManager.h"
#include "GraphicConst.h"
using namespace std;

LPSpriteManager CSpriteManager::instance = NULL;

CSpriteManager::CSpriteManager()
{

}

void CSpriteManager::Init()
{
	LoadSprite(SPRITE_DATABASE + MARIO_DB);
}

void CSpriteManager::Add(string id, RECT rect, LPDIRECT3DTEXTURE9 tex)
{
	LPSprite s = new CSprite(id, rect, tex);
	sprites.at(id) = s;
}

bool CSpriteManager::LoadSprite(string filePath)
{
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

}