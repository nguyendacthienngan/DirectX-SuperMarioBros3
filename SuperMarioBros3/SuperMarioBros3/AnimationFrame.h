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
	D3DXVECTOR2 position;
	// pos để làm gì: Để animation frame lưu trữ vị trí của sprite. VD những animation đặc biệt cần xác định vị trí của mỗi sprite đó ở nhiều vị trí khác nhau
public:
	CAnimationFrame(LPSprite sprite, DWORD time, D3DXVECTOR2 position);
	LPSprite GetSprite();
	DWORD GetTime();
	~CAnimationFrame();
};

#endif