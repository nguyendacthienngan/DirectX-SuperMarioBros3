#pragma once
#include "GameObject.h"
#include "PhysicsBody.h"
#include "RectF.h"

// Vật mà Body sẽ dùng để nhận ra sự va chạm. Găn liền với mỗi game object

class CGameObject;
typedef CGameObject* LPGameObject;

class CPhysicsBody;
typedef CPhysicsBody* LPPhysicsBody;

class CCamera;

class CCollisionBox
{
private:
	LPGameObject gameObject;
	D3DXVECTOR2 sizeBox;
	D3DXVECTOR2 localPosition; 
	D3DXVECTOR2 distance;  //dx = vx*dt 
	std::string name;
	int id;

public:
	CCollisionBox();
	~CCollisionBox();
	void Render(CCamera* camera, int distance);
	
	void SetGameObjectAttach(LPGameObject gO);
	LPGameObject GetGameObjectAttach();
	
	void SetSizeBox(D3DXVECTOR2 size);
	D3DXVECTOR2 GetSizeBox();

	void SetPosition(D3DXVECTOR2 pos); // Local Position
	D3DXVECTOR2 GetPosition();

	void SetDistance(D3DXVECTOR2 d);
	D3DXVECTOR2 GetDistance();

	D3DXVECTOR2 GetWorldPosition();

	bool IsEnabled();

	RectF GetBoundingBox();

	void SetName(std::string n);
	std::string GetName();

	void SetId(int i);
	int GetID();
};
typedef CCollisionBox* LPCollisionBox;