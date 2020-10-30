#include "AnimationManager.h"
#include "Ultis.h"
#include "tinyxml.h"
#include "SpriteManager.h"
#include "GraphicConst.h"
#include "Game.h"
#include "Const.h"
using namespace std;
//Tạm thời
#include "TextureManager.h"
#include "Animation.h"
LPAnimationManager CAnimationManager::instance = NULL;

LPAnimationManager CAnimationManager::GetInstance()
{
    if (instance == NULL) instance = new CAnimationManager();
    return instance;
}

CAnimationManager::~CAnimationManager()
{
	
}

CAnimationManager::CAnimationManager()
{
}

bool CAnimationManager::Init()
{
	DebugOut(L"[INFO] Load Animation from file XML \n");
	auto root = CGame::GetInstance();
	if (InitAnAnimationSet(TEXTURE_MARIO, root->GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_MARIO)) == false)
	{
		DebugOut(L"Cannot Load Mario Animation File \n");
		return false;
	}
	if (InitAnAnimationSet(TEXTURE_FIRE_BALL, root->GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_FIRE_BALL)) == false)
	{
		DebugOut(L"Cannot Load Fire Ball Animation File \n");
		return false;
	}
	if (InitAnAnimationSet(TEXTURE_ENEMY, root->GetFilePathByCategory(CATEGORY_ANIMATION, DB_ANIMATION_ENEMY)) == false)
	{
		DebugOut(L"Cannot Load Fire Ball Animation File \n");
		return false;
	}
	return true;
}

bool CAnimationManager::InitAnAnimationSet(std::string textureName, std::string filePath)
{
	if (LoadAnimation(textureName, filePath) == false)
	{
		DebugOut(L"Cannot Load", textureName + "Animation File \n");
		return false;
	}
	return true;
}

bool CAnimationManager::LoadAnimation(std::string texName, std::string filePath)
{
	// Load from XML
	OutputDebugStringW(ToLPCWSTR(filePath.c_str())); 
	DebugOut(L"\n");

	TiXmlDocument document(filePath.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();

	for (TiXmlElement* info = root->FirstChildElement(); info != nullptr; info = info->NextSiblingElement())
	{
		string gameObjectID = info->Attribute("gameObjectId");
		string textureID = info->Attribute("textureId");
		if (textureID != texName)
			continue;

		LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->GetTexture(textureID);
		if (tex != nullptr)
			OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));
		else
			return false;

		OutputDebugStringW(ToLPCWSTR("Gameobject id: " + gameObjectID + '\n'));
		OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));

		for (TiXmlElement* node = info->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
		{
			string aniId = node->Attribute("aniId");
			int frameTime;
			node->QueryIntAttribute("frameTime", &frameTime);
			string name = node->Attribute("name");
			OutputDebugStringW(ToLPCWSTR(aniId + ':' + to_string(frameTime) + ':' + name + '\n'));
			LPAnimation animation = new CAnimation(aniId, frameTime);

			// Sprite ref
			for (TiXmlElement* sprNode = node->FirstChildElement(); sprNode != nullptr; sprNode = sprNode->NextSiblingElement())
			{
				string id = sprNode->Attribute("id");
				LPSprite sprite = CSpriteManager::GetInstance()->Get(id);
				animation->Add(sprite, D3DXVECTOR2(0.0f, 0.0f), frameTime);
				OutputDebugStringW(ToLPCWSTR("|--" + id + ':' + to_string(frameTime) + '\n'));
			}

			AddAnimation(aniId, animation);
		}
		return true;
	}
	
}

void CAnimationManager::AddAnimation(std::string id, LPAnimation ani)
{
	animations.insert(make_pair(id, ani));
}

LPAnimation CAnimationManager::Get(std::string id)
{
	LPAnimation ani = animations.at(id);
	if (ani == NULL)
	{
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
		return nullptr;
	}	
	return ani;
}

LPAnimation CAnimationManager::Clone(std::string id)
{
	LPAnimation oriAni = Get(id);
	LPAnimation copiedAni = new CAnimation(*oriAni);
	return copiedAni;
}


void CAnimationManager::Clear()
{
	for (auto x : animations)
	{
		LPAnimation ani = x.second;
		delete ani;
	}
	animations.clear();
}

