#pragma once
#include "GameObject.h"
class CGhostPlatform: public CGameObject
{
	// Đối với các GhostPlatform, ta sẽ xét va chạm khác với các dạng solid, vì solid ta sẽ xét va chạm với 4 hướng
	// Còn ghost platform ta sẽ xét va chạm với một số hướng thôi (Như là xét ở trên top )
	// Do đó ta sẽ cần đến Effector để quyết định xem có ảnh hưởng va chạm lên mặt nào của từ bên ngoài của collisionbox
public:
	CGhostPlatform();
	void Init() override;
	bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag) override;
	~CGhostPlatform();
};

