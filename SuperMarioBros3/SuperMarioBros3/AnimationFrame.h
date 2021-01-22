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
	DWORD time; // frameTime : Thời gian sprite đó được chạy
public:
	CAnimationFrame(LPSprite sprite, DWORD time);
	LPSprite GetSprite();
	DWORD GetTime();
	~CAnimationFrame();
};

#endif