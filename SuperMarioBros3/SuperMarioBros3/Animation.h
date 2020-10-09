#pragma once
#ifndef ANIMATINO_H
#define ANIMATION_H

#include <string>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

#include "AnimationFrame.h"
#include "Sprite.h"

class CAnimation;
typedef CAnimation* LPAnimation;
class CAnimation
{
private:
	std::string id;
	DWORD defaultFrameTime, lastFrameTime;
	int currentFrame;
	float speedMultiplier;
	std::vector<LPAnimationFrame> animFrames;

public: 
	CAnimation(DWORD defaultTime = 100);
	CAnimation(const CAnimation& obj);
	void Add(LPSprite sprite, D3DXVECTOR2 pos, DWORD frameTime = 0);
	void Update();
	void Render(D3DXVECTOR2 position, int alpha = 255);


	void SetSpeedMultiplier(float speed) { speedMultiplier = speed; }
	void ResetSpeedMultiplier() { speedMultiplier = 1.0f; }
	float GetSpeedMultiplier() { return speedMultiplier; }

};

#endif