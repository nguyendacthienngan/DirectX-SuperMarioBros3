#pragma once
enum class ItemTag
{
	Coin,
	PowerUp,
	PSwitch,
	None
};

struct ItemInfo
{
	ItemTag tag;
	int quantity;
};

enum class PowerupTag
{
	SuperMushroom,
	SuperLeaf,
	FireFlower,
	None
};