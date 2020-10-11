#pragma once

#ifndef ANIMATINO_H
#define ANIMATION_H

#include <string>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

#include "AnimationFrame.h"
#include "Sprite.h"
#include "Transform.h"


class CAnimation;
typedef CAnimation* LPAnimation;
class CAnimation
{
private:
	std::string id;
	DWORD defaultFrameTime, lastFrameTime;
	int currentFrame;
	float speedMultiplier;

	Transform transform;

	std::vector<LPAnimationFrame> animFrames;

public: 
	CAnimation(std::string aniID, DWORD defaultTime = 100);
	CAnimation(const CAnimation& obj);
	void Add(LPSprite sprite, D3DXVECTOR2 pos, DWORD frameTime = 0);
	void Update();
	void Render(D3DXVECTOR2 position, int alpha = 255);

	void SetPosition(D3DXVECTOR2 p) { this->transform.translatePos = p; }
	void SetScale(D3DXVECTOR2 s) { this->transform.scale = s; }
	void SetRotation(float r) { this->transform.rotationAngle = r; }

	D3DXVECTOR2 GetPosition() { return transform.translatePos; }
	D3DXVECTOR2 GetScale() { return transform.scale; }
	float GetRotation() { return transform.rotationAngle; }

	void SetState();

	void SetSpeedMultiplier(float speed) { speedMultiplier = speed; }
	void ResetSpeedMultiplier() { speedMultiplier = 1.0f; }
	float GetSpeedMultiplier() { return speedMultiplier; }

	~CAnimation();
};

#endif
