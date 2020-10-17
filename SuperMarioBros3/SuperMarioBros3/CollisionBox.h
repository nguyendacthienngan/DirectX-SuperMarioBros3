#pragma once
#include "GameObject.h"
#include "PhysicsBody.h"
#include "RectF.h"

// Vật mà Body sẽ dùng để nhận ra sự va chạm. Găn liền với mỗi game object

class CGameObject;
typedef CGameObject* LPGameObject;

class CPhysicsBody;
typedef CPhysicsBody* LPPhysicsBody;

class CCollisionBox
{
private:
	LPGameObject gameObject;
	D3DXVECTOR2 sizeBox;
	D3DXVECTOR2 localPosition; 
	D3DXVECTOR2 distance;  //dx = vx*dt
public:

	void SetGameObjectAttach(LPGameObject gO);
	LPGameObject GetGameObjectAttach();
	
	void SetSizeBox(D3DXVECTOR2 size);
	D3DXVECTOR2 GetSizeBox();

	void SetPosition(D3DXVECTOR2 pos);
	D3DXVECTOR2 GetPosition();

	void SetDistance(D3DXVECTOR2 d);
	D3DXVECTOR2 GetDistance();

	D3DXVECTOR2 GetWorldPosition();

	bool IsEnabled();

	RectF GetBoundingBox();

};
typedef CCollisionBox* LPCollisionBox;