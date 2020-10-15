#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Animation.h"

class CGameObject;
typedef CGameObject* LPGameObject;

struct CCollisionEvent;
typedef CCollisionEvent* LPCollisionEvent;

class CGameObject
{
protected:
	int id;
	DWORD dt;

	Transform transform;

	//D3DXVECTOR2 position;
	D3DXVECTOR2 velocity;
	D3DXVECTOR2 distance; // dx = vx*dt, dy = vy*dt, độ dời sau một khoảng thời gian
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
public:
	CGameObject();
	~CGameObject();

	// TO-DO
	virtual void Init(); // sẽ thuần ảo để đến từng gameObject cụ thể sẽ tự load animation riêng
	virtual void Update(DWORD dt, std::vector<LPGameObject>* coObjects = NULL); // Truyền một vector GameObject để xét va chạm với GameObject hiện tại. Tạm thời chưa làm Update do chưa xử lý va chạm
	virtual void Render();
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

	// COLLISION
	void RenderBoundingBox();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;


	LPCollisionEvent SweptAABBEx(LPGameObject coO);
	void CalcPotentialCollisions(std::vector<LPGameObject>* coObjects, std::vector<LPCollisionEvent>& coEvents);
	void FilterCollision(
		std::vector<LPCollisionEvent>& coEvents,
		std::vector<LPCollisionEvent>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);


	void SetPosition(D3DXVECTOR2 p) { this->transform.translatePos = p; }
	void SetSpeed(D3DXVECTOR2 v) { this->velocity = v; }

	D3DXVECTOR2 GetPosition() { return transform.translatePos; }
	D3DXVECTOR2 GetSpeed() { return velocity; }

	D3DXVECTOR2 GetScale() { return transform.scale; }
	float GetRotation() { return transform.rotationAngle; }

	D3DXVECTOR2 GetDistance() { return distance; }

	void SetState(std::string state);

};


struct CCollisionEvent
{
	LPGameObject obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGameObject obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCollisionEvent& a, LPCollisionEvent& b)
	{
		return a->t < b->t;
	}
};

#endif