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
#include "Scene.h"

class CScene;
typedef CScene* LPScene;

class CPhysicsBody;
typedef CPhysicsBody* LPPhysicsBody;

class CCollisionBox;
typedef CCollisionBox* LPCollisionBox;

class CGameObject;
typedef CGameObject* LPGameObject;

class CCamera;
typedef CCamera* LPCamera;

struct CollisionEvent;

class CAnimation;
typedef CAnimation* LPAnimation;

// Mỗi gameobject sẽ lưu animation của riêng nó. Nó sẽ clone animation từ animation gốc chứ k lấy thẳng con trỏ animation bên đó
// Vì như vậy sẽ làm cho việc animation quá đồng bộ và không tự nhiên
// Nhưng lưu ý khi cloneanimation và setstate nhớ đặt tên giống nhau !

class CGameObject
{
protected:
	int id;
	DWORD dt;

	Transform transform; // position ( dời hình ), scale, rotate
	D3DXVECTOR2 relativePositionOnScreen = D3DXVECTOR2(0.0f, 0.0f);

	std::string currentState, lastState;

	bool isEnabled;
	bool ignoreTimeScale;
	GameObjectTags tag; // Phân biệt player với eniemies,...
	std::unordered_map<std::string, LPAnimation> animations;
	
	std::vector<LPCollisionBox>* collisionBoxs;
	LPPhysicsBody physiscBody;
	bool isCheckWithCollision;
public:
	CGameObject();
	~CGameObject();

	virtual void Init();
	virtual void Clear();
	virtual void LoadAnimation();

	virtual void PhysicsUpdate(std::vector<LPGameObject>* coObjects);
	virtual void Update(DWORD dt, CCamera* cam);
	virtual void Render(CCamera* cam, int alpha = 255);

	void FrictionProcess(float& speed, DWORD dt);
	void ResetTempValues();
	// Collision
	// Khi PhysicsBody bắt dược va chạm, nó sẽ tùy thuộc vào biến trigger để truyền về đúng loại callback mình muốn
	// OnCollisionEnter: Khi bắt đc va chạm => Hàm này sẽ có hiện tượng vật lý xảy ra như khiến cho mario dội ra khỏi cục gạch,.. kèm theo event trả về
	// OnTriggerEnter: Chỉ trả về event thôi, không có hiện tượng vật lý, xử lý gì cả => Dành cho các portal, game bắn máy bay,...

	virtual void OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
	virtual void OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
	virtual void OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox);
	virtual void OnDie();
	virtual void OnDamaged();

	void AddAnimation(std::string stateName, LPAnimation animation, bool isLoop = true); // một số animation như quăng lửa k cần lặp
	void SetRelativePositionOnScreen(D3DXVECTOR2); // Dùng khi ví dụ chuyển từ idle -> crouch, dời hình xuống
	virtual void EndAnimation();
	LPAnimation GetAnimationByState(std::string state);

	virtual void KeyState();
	virtual void OnKeyDown(int KeyCode); // sẽ thuần ảo để đến từng gameObject cụ thể sẽ tự xử lý
	virtual void OnKeyUp(int KeyCode);

	bool IsEnabled();
	void Enable(bool isEnabled);

	bool IsIgnoreTimeScale();
	void SetIgnoreTimeSCale(bool isIgnoreTimeScale);

	D3DXVECTOR2 GetScale();
	void SetScale(D3DXVECTOR2 s);

	float GetRotation();
	void SetRotation(float r);

	D3DXVECTOR2 GetPosition();
	void SetPosition(D3DXVECTOR2 p);

	LPPhysicsBody GetPhysiscBody();
	void SetPhysiscBody(LPPhysicsBody p);

	std::vector<LPCollisionBox>* GetCollisionBox();
	void GetCollisionBox(std::vector<LPCollisionBox>* listCollisionBox);

	std::string GetState();
	void SetState(std::string state);

	GameObjectTags GetTag();
	void SetTag(GameObjectTags t);

	virtual void AddObjectToScene(LPScene scene);

	virtual bool CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag);
	bool MarioTag(GameObjectTags tag);
	bool StaticTag(GameObjectTags tag);
	bool GiftTag(GameObjectTags tag);
	bool PlayerAttackItemTag(GameObjectTags tag);
};

#endif