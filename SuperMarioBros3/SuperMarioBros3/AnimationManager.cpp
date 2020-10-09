#include "AnimationManager.h"
#include "Ultis.h"
#include "tinyxml.h"
#include "SpriteManager.h"
#include "GraphicConst.h"
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

CAnimationManager::CAnimationManager()
{
}

bool CAnimationManager::Init()
{
	DebugOut(L"[INFO] Load Animation from file XML \n");
	if (InitAnAnimationSet(ANIMATION_PATH + MARIO_ANIM) == false)
	{
		DebugOut(L"Cannot Load Mario Animation File \n");
		return false;
	}
	/*if (InitAnAnimation("EnemyAnim") == false)
	{
		DebugOut(L"Cannot Load Enemy Animation File");
		return false;
	}
	if (InitAnAnimation("MiscAnim") == false)
	{
		DebugOut(L"Cannot Load Misc Animation File");
		return false;
	}
	if (InitAnAnimation("UiAnim") == false)
	{
		DebugOut(L"Cannot Load UI Animation File");
		return false;
	}*/
	return true;
}

bool CAnimationManager::InitAnAnimationSet(std::string textureName)
{
	if (LoadAnimation(textureName) == false)
	{
		DebugOut(L"Cannot Load", textureName + "Animation File \n");
		return false;
	}
	return true;
}

bool CAnimationManager::LoadAnimation(std::string filePath)
{
	// test
	/*LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->GetTexture("tex-mario");
	LPAnimation ani = new CAnimation("Run");
	RECT r;
	r.left = 323;
	r.top = 8;
	r.right = 323 + 16;
	r.bottom = 8 + 16;

	LPSprite spr = new CSprite("spr-small-mario-run-0", r, tex);
	ani->Add(spr, D3DXVECTOR2(2.0f, 2.5f), 100);

	r.left = 341;
	r.right = 341 + 16;
	spr = new CSprite("spr-small-mario-run-1", r, tex);
	ani->Add(spr,D3DXVECTOR2(2.0f, 2.5f), 100);
	AddAnimation("ani-small-mario-run",ani);*/

	// Load from XML
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
	TiXmlElement* info = root->FirstChildElement();

	string gameObjectID = info->Attribute("gameObjectId");
	string textureID = info->Attribute("textureId");

	OutputDebugStringW(ToLPCWSTR("Gameobject id: " + gameObjectID + '\n'));
	OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));

	for (TiXmlElement* node = info->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
	{
		string aniId = node->Attribute("aniId");
		float frameTime;
		node->QueryFloatAttribute("frameTime", &frameTime);
		string name = node->Attribute("name");
		OutputDebugStringW(ToLPCWSTR(aniId + ':' + to_string(frameTime) + ':' + name + '\n'));
		LPAnimation animation = new CAnimation(aniId, frameTime);

		// Sprite ref
		for (TiXmlElement* sprNode = node->FirstChildElement(); sprNode != nullptr; sprNode = sprNode->NextSiblingElement())
		{
			string id = sprNode->Attribute("id");
			LPSprite sprite = CSpriteManager::GetInstance()->Get(id);
			float detailFrameTime;
			sprNode->QueryFloatAttribute("frameTime", &detailFrameTime);
			animation->Add(sprite, D3DXVECTOR2(0.0f,0.0f), detailFrameTime);

			OutputDebugStringW(ToLPCWSTR("|--" + id + ':' + to_string(detailFrameTime) + '\n'));
		}

		AddAnimation(aniId, animation);
	}
	return true;
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

