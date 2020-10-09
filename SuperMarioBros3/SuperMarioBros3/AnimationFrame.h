#pragma once
#ifndef ANIMATIONFRAME_H
#define ANIMATIONFRAME_H
#include <Windows.h>
#include "Sprite.h"

class CAnimationFrame;
typedef CAnimationFrame* LPAnimationFrame;
class CAnimationFrame
{
private:
	LPSprite sprite;
	DWORD time;
	D3DXVECTOR2 position;
public:
	CAnimationFrame(LPSprite sprite, DWORD time, D3DXVECTOR2 position);
	LPSprite GetSprite();
	DWORD GetTime();
};

#endif