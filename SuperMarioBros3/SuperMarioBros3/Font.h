#pragma once
#include <map>
#include <vector>
#include "Sprite.h"
class CFont
{
private:
	std::map<char, LPSprite> fonts;
	std::string currentText;
	D3DXVECTOR2 currentPosition;
	D3DXVECTOR2 space;
public:
	CFont();
	void LoadSprite();
	void Render();
	LPSprite CharToSprite(char character);
	std::vector<LPSprite> StringToSprites(std::string str);
	bool InScope(char character);
	bool IsLetterLowerCase(char character);
	void SetCurrentText(std::string text);
	void SetPosition(D3DXVECTOR2 pos);
	void SetSpace(D3DXVECTOR2 space);
};

