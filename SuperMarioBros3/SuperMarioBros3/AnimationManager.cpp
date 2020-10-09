#include "AnimationManager.h"
#include "Ultis.h"

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
	if (InitAnAnimationSet("MarioAnim") == false)
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
	LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->GetTexture("tex-mario");
	LPAnimation ani = new CAnimation();
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
	AddAnimation("ani-small-mario-run",ani);
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

