#pragma once
#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <unordered_map>

#include "Animation.h"

class CAnimationManager;
typedef CAnimationManager* LPAnimationManager;
class CAnimationManager
{
private:
	static LPAnimationManager instance;
	std::unordered_map<std::string, LPAnimation> animations; // Lưu toàn bộ 
	//std::unordered_map<std::string, LPAnimationSet> animationSets; // Sẽ lưu <id của animation set, animationSet>
public:
	CAnimationManager();
	bool Init();
	bool InitAnAnimationSet(std::string textureName);
	bool LoadAnimation(std::string filePath);
	void AddAnimation(std::string id, LPAnimation ani); // Lưu toàn bộ DB
	//void AddAnimationSet(std::string aniSetID, LPAnimationSet aniSet); // Tách ra theo từng set
	LPAnimation Get(std::string id);
	LPAnimation Clone(std::string id); // Copy constructor: Giúp copy ani ra bản sao để mỗi object xài ani riêng. Không bị trùng nhau
	void Clear();
	static LPAnimationManager GetInstance();
};
#endif

