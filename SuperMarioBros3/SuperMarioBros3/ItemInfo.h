#pragma once
enum class ItemTag
{
	Coin,
	SuperMushroom,
	SuperLeaf,
	FireFlower,
	None
};

struct ItemInfo
{
	ItemTag tag;
	int quantity;
};