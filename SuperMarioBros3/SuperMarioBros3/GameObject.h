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
#include "GameObjectTags.h"
#include "Effector.h"

class CPhysicsBody;
typedef CPhysicsBody* LPPhysicsBody;


class CCollisionBox;
typedef CCollisionBox* LPCollisionBox;

class CGameObject;
typedef CGameObject* LPGameObject;

class CCamera;
typedef CCamera* LPCamera;

class CollisionEvent;

class CGameObject
{
protected:
	int id;
	DWORD dt;

	Transform transform; // position ( dời hình ), scale, rotate
	D3DXVECTOR2 relativePositionOnScreen = D3DXVECTOR2(0.0f, 0.0f);

	std::string currentState;

	bool isEnabled;

	GameObjectTags tag; // Phân biệt player với eniemies,...
	Effector effector;
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

	virtual void Init(); // sẽ thuần ảo để đến từng gameObject cụ thể sẽ tự load animation riêng
	virtual void LoadAnimation();

	void PhysicsUpdate(std::vector<LPGameObject>* coObjects);
	virtual void Update(DWORD dt, CCamera* cam);
	virtual void Render(CCamera* cam);
	//virtual void AnimationUpdate();

	// Collision
	// Khi PhysicsBody bắt dược va chạm, nó sẽ tùy thuộc vào biến trigger để truyền về đúng loại callback mình muốn
	// OnCollisionEnter: Khi bắt đc va chạm => Hàm này sẽ có hiện tượng vật lý xảy ra như khiến cho mario dội ra khỏi cục gạch,.. kèm theo event trả về
	virtual void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
	// OnTriggerEnter: Chỉ trả về event thôi, không có hiện tượng vật lý, xử lý gì cả => Dành cho các portal, game bắn máy bay,...
	virtual void OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);

	// Animation
	void AddAnimation(std::string stateName, LPAnimation animation);
	void SetRelativePositionOnScreen(D3DXVECTOR2); // Dùng khi ví dụ chuyển từ idle -> crouch, dời hình xuống
	// Keyboard
	virtual void KeyState();
	virtual void OnKeyDown(int KeyCode); // sẽ thuần ảo để đến từng gameObject cụ thể sẽ tự xử lý
	virtual void OnKeyUp(int KeyCode);

	bool IsEnabled();
	void Enable(bool isEnabled) { this->isEnabled = isEnabled; }

	// Transform
	void SetScale(D3DXVECTOR2 s) { this->transform.scale = s; }
	void SetRotation(float r)	 { this->transform.rotationAngle = r; }
	void SetPosition(D3DXVECTOR2 p) { this->transform.position = p; }

	D3DXVECTOR2 GetPosition() { return this->transform.position;}
	D3DXVECTOR2 GetScale() { return transform.scale; }
	float GetRotation() { return transform.rotationAngle; }
	LPAnimation GetAnimationByState(std::string state);
	LPPhysicsBody GetPhysiscBody() { return physiscBody; }
	std::vector<LPCollisionBox>* GetCollisionBox() { return collisionBoxs; }
	void GetCollisionBox(std::vector<LPCollisionBox>* listCollisionBox) { this->collisionBoxs = listCollisionBox; }
	GameObjectTags GetTag() { return tag; }
	Effector GetEffector() { return effector; }

	void SetState(std::string state);
	void SetPhysiscBody(LPPhysicsBody p) { this->physiscBody = p; }
	void SetTag(GameObjectTags t) { this->tag = t; }
	void SetEffector(Effector e) { this->effector = effector; }
};

#endif