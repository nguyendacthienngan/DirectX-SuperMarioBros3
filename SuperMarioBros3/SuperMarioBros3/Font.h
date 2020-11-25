#pragma once
#include <map>
#include <vector>
#include "Sprite.h"
class CFont
{
private:
	std::map<char, LPSprite> fonts;
public:
	CFont();
	void LoadSprite();
	void Render();
	LPSprite CharToSprite(char character);
	std::vector<LPSprite> StringToSprites(std::string str);
	bool InScope(char character);
	bool IsLetterLowerCase(char character);
};

