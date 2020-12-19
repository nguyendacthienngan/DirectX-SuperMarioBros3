#include "CardGift.h"
#include "SpriteManager.h"
using namespace std;
CCardGift::CCardGift()
{
	LoadSprite();
	state = "EMPTY";
}

CCardGift::CCardGift(D3DXVECTOR2 cardPos)
{
	LoadSprite();
	pos = cardPos;
	state = "EMPTY";
}

void CCardGift::LoadSprite()
{
	auto spriteManager = CSpriteManager::GetInstance();
	auto sprite = spriteManager->Get("spr-empty-card-0");
	cardSprites.insert(make_pair("EMPTY", sprite));

	sprite = spriteManager->Get("spr-fire-flower-card-0");
	cardSprites.insert(make_pair("FLOWER", sprite));

	sprite = spriteManager->Get("spr-star-man-card-0");
	cardSprites.insert(make_pair("STAR", sprite));

	sprite = spriteManager->Get("spr-super-mushroom-card-0");
	cardSprites.insert(make_pair("MUSHROOM", sprite));
}

void CCardGift::Update()
{
}

void CCardGift::Render()
{
	// TO-DO: Render nhấp nháy !
	if (cardSprites.find(state) != cardSprites.end())
		cardSprites.at(state)->Draw(pos, D3DXVECTOR2(1.0f, 1.0f), 0.0f);
}

void CCardGift::SetState(std::string state)
{
	this->state = state;
}

void CCardGift::SetPosition(D3DXVECTOR2 pos)
{
	this->pos = pos;
}
