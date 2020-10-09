#include "AnimationFrame.h"

CAnimationFrame::CAnimationFrame(LPSprite sprite, DWORD time, D3DXVECTOR2 position)
{
    this->sprite = sprite; 
    this->time = time;
    this->position = position;
}

LPSprite CAnimationFrame::GetSprite()
{
    return sprite;
}

DWORD CAnimationFrame::GetTime()
{
    return time;
}
