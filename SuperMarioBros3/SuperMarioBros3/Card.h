#pragma once
#include "GameObject.h"
class CGameObject;
class CScene;
class CCard :  public CGameObject
{
private:
	int cardState; // 0: Flower, 5: Star, 10: Mushroom
	bool isTouched;
public:
	CCard();
	void LoadAnimation() override;
	void SetCardState(int state);

	void Render(CCamera* cam, int alpha);
	virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	virtual void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
	virtual void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox);
	virtual bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag);
	void IsTouched(bool state);
	bool IsTouched();
};

