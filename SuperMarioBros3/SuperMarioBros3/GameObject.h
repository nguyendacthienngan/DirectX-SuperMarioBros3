#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Animation.h"
#include "Camera.h"
#include "PhysicsBody.h"
#include "CollisionBox.h"


class CPhysicsBody;
typedef CPhysicsBody* LPPhysicsBody;


class CCollisionBox;
typedef CCollisionBox* LPCollisionBox;

class CGameObject;
typedef CGameObject* LPGameObject;

class CGameObject
{
protected:
	int id;
	DWORD dt;

	Transform transform;

	//D3DXVECTOR2 position;
	D3DXVECTOR2 velocity;
	//D3DXVECTOR2 distance; // dx = vx*dt, dy = vy*dt, độ dời sau một khoảng thời gian
	D3DXVECTOR2 normal; // vector pháp tuyến? nx, ny
	float acceleration; // gia tốc

	std::string currentState;

	// To-Do
	bool isEnabled;

	std::string tag; // Phân biệt player với eniemies,...

	// Mỗi gameobject sẽ lưu animation của riêng nó. Nó sẽ clone animation từ animation gốc chứ k lấy thẳng con trỏ animation bên đó
	// Vì như vậy sẽ làm cho việc animation quá đồng bộ và không tự nhiên
	// Nhưng lưu ý khi cloneanimation và setstate nhớ đặt tên giống nhau !

	std::unordered_map<std::string, LPAnimation> animations;
	
	// Khi áp dụng PhysicBody lên vật nào thì vật đó sẽ có va chạm với vật khác nếu vật đó có CollisionBox
	// VD: Mario = Body + CollisionBox, Ghost = Body, Pipe = CollisionBox

	std::vector<LPCollisionBox>* collisionBoxs;
	LPPhysicsBody physiscBody;
	
public:
	CGameObject();
	~CGameObject();

	// TO-DO
	virtual void Init(); // sẽ thuần ảo để đến từng gameObject cụ thể sẽ tự load animation riêng
	virtual void Update(DWORD dt, CCamera* cam, std::vector<LPGameObject>* coObjects = NULL); // Truyền một vector GameObject để xét va chạm với GameObject hiện tại. Tạm thời chưa làm Update do chưa xử lý va chạm
	virtual void Render(CCamera* cam);
	virtual void AnimationUpdate();

	void AddAnimation(std::string stateName, LPAnimation animation);
	std::string GetTag() { return tag; }
	// Keyboard
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0; // sẽ thuần ảo để đến từng gameObject cụ thể sẽ tự xử lý
	virtual void OnKeyUp(int KeyCode) = 0;

	bool IsEnabled();

	// TRANSFORM
	void SetScale(D3DXVECTOR2 s) 
	{ 
		this->transform.scale = s;
		this->animations[currentState]->SetScale(s); // Khi setscale ở GameObject phải đồng bộ với Animation
	}
	void SetRotation(float r) 
	{
		this->transform.rotationAngle = r; 
		this->animations[currentState]->SetRotation(r); // Khi setrotation ở GameObject phải đồng bộ với Animation
	}

	void RenderBoundingBox();

	void SetPosition(D3DXVECTOR2 p) { this->transform.position = p; }
	void SetSpeed(D3DXVECTOR2 v) { this->velocity = v; }

	D3DXVECTOR2 GetPosition() { return this->transform.position;}
	D3DXVECTOR2 GetSpeed() { return velocity; }

	D3DXVECTOR2 GetScale() { return transform.scale; }
	float GetRotation() { return transform.rotationAngle; }

	void SetState(std::string state);

	LPPhysicsBody GetPhysiscBody() { return physiscBody; }
	void SetPhysiscBody(LPPhysicsBody p) { this->physiscBody = p; }

	std::vector<LPCollisionBox>* GetCollisionBox() { return collisionBoxs; }
	void GetCollisionBox(std::vector<LPCollisionBox>* listCollisionBox) { this->collisionBoxs = listCollisionBox; }
};

#endif