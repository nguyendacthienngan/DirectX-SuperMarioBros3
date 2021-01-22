#include "AnimationFrame.h"

CAnimationFrame::CAnimationFrame(LPSprite sprite, DWORD time)
{
    this->sprite = sprite; 
    this->time = time;
}

LPSprite CAnimationFrame::GetSprite()
{
    return sprite;
}

DWORD CAnimationFrame::GetTime()
{
    return time;
}

CAnimationFrame::~CAnimationFrame()
{
}
