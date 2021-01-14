#pragma once
#include "Font.h"
#include "GameObject.h"
class CMenu : public CGameObject
{
private:
	CFont* selection1;
	CFont* selection2;
	CSprite* arrow;
	bool isUp;
	std::map<std::string, D3DXVECTOR2> direction;
	D3DXVECTOR2 currentDirection;
public:
	CMenu();

	void Render(CCamera* cam, int alpha = 255) override;
	virtual void OnKeyDown(int KeyCode); 
	virtual void OnKeyUp(int KeyCode);
};

