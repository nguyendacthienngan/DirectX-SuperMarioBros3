#pragma once
enum class ItemTag
{
	Coin,
	SuperMushroom,
	SuperLeaf,
	FireFlower
};

struct ItemInfo
{
	ItemTag tag;
	int quantity;
};