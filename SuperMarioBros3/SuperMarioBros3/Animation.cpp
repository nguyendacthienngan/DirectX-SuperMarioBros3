#include <Windows.h>

#include "Animation.h"
#include "Game.h"
#include <vector>
#include <string>
#include "Ultis.h"
using namespace std;

CAnimation::CAnimation(string aniID, DWORD defaultTime)
{
	id = aniID;
	currentFrame = -1;
	speedMultiplier = 1;
	this->defaultFrameTime = defaultTime;
}

CAnimation::CAnimation(const CAnimation& obj)
{
	this->id = obj.id;
	this->defaultFrameTime = obj.defaultFrameTime;
	this->lastFrameTime = obj.lastFrameTime;
	this->currentFrame = obj.currentFrame;
	this->speedMultiplier = 1.0f;
	std::vector<LPAnimationFrame> newFrames(obj.animFrames);
	this->animFrames = newFrames;
	//this->transform = obj.transform;
	this->transform = transform.CreateTransformation(obj.transform.translatePos, obj.transform.scale, obj.transform.rotationAngle);
}

void CAnimation::Add(LPSprite sprite, D3DXVECTOR2 pos, DWORD frameTime)
{
	// pos để làm gì: Để animation frame lưu trữ vị trí của sprite. VD những animation đặc biệt cần xác định vị trí của mỗi sprite đó ở nhiều vị trí khác nhau
	int t = frameTime;
	if (frameTime == 0) t = this->defaultFrameTime;

	LPAnimationFrame frame = new CAnimationFrame(sprite, t, pos);
	animFrames.push_back(frame);
}

void CAnimation::Update()
{
	DebugOut(L"[INFO] Animation Update \n");
	//if (animFrames.size() == 0 || speedMultiplier == 0.0f) return;
	if (animFrames.size() == 0) return;

	DWORD currentTime = GetTickCount();

	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = currentTime;
	}
	else
	{
		int frameInCurrentFrame = animFrames[currentFrame]->GetTime();
		string s = std::to_string(frameInCurrentFrame);
		DebugOut(ToLPCWSTR("Frame Time : " + s + "\n"));
		DebugOut(ToLPCWSTR("Time Scale : " + std::to_string(CGame::GetTimeScale()) + "\n"));
		DebugOut(ToLPCWSTR("speedMultiplier : " + std::to_string(speedMultiplier) + "\n"));

		if (currentTime - lastFrameTime > animFrames[currentFrame]->GetTime() * CGame::GetTimeScale() / speedMultiplier)
		{
			DebugOut(ToLPCWSTR("Current Frame: " + std::to_string(currentTime)  +"\n"));
			currentFrame = (currentFrame + 1) % animFrames.size();
			lastFrameTime = currentTime;
			DebugOut(ToLPCWSTR("LastFrameTime: " + std::to_string(currentTime) + "\n"));

		}
	}
}

void CAnimation::Render(D3DXVECTOR2 position, int alpha)
{
	DebugOut(L"[INFO] Render Animation \n");
	OutputDebugString(ToLPCWSTR("Position: " + std::to_string(position.x) + "\n"));
	D3DXVECTOR2 translateDestination = D3DXVECTOR2 (position.x, position.y);
	transform.translatePos = translateDestination;
	OutputDebugString(ToLPCWSTR("Translate Position: " + std::to_string(transform.translatePos.x) + "\n"));
	animFrames[currentFrame]->GetSprite()->Draw(transform.translatePos, transform.scale, transform.rotationAngle, alpha);
}

CAnimation::~CAnimation()
{
	for (auto a : animFrames)
		delete a;
	animFrames.clear();
}
