#pragma once
//#include "GameObject.h"
#include "CollisionBox.h"
#include <d3d9.h>
#include <d3d9.h>
#include "GameObjectTags.h"
struct CollisionEvent;
typedef CollisionEvent* LPCollisionEvent;

// PhysicBody đảm nhiệm vụ mô phỏng chuyển động VẬT LÝ của Game Object => Xử lý va chạm
// Chứ không dùng để dụng chạm với thằng nào hết => Chuyện đó của CollisionBox
// Khi áp dụng PhysicBody lên vật nào thì vật đó sẽ có va chạm với vật khác nếu vật đó có CollisionBox

// VD: Mario = Body + CollisionBox, Ghost = Body, Pipe = CollisionBox

// Lưu ý Body khác với Transform, Transform chỉ là phép biến hình, không ràng buộc bởi trọng lực, khối lượng, vận tốc
// Body sẽ thay đổi Transform nhưng không ngược lại

//class CGameObject;
//typedef CGameObject* LPGameObject;

class CCollisionBox;
typedef CCollisionBox* LPCollisionBox;

class CGameObject;
typedef CGameObject* LPGameObject;

class CPhysicsBody
{
private:
	D3DXVECTOR2 velocity; // vận tốc
	float gravity; // trọng lực (gia tốc trọng trường)
	float acceleration; // gia tốc
	D3DXVECTOR2 bounceForce; // lực nảy

	D3DXVECTOR2 dragForce; // lực kéo: tăng theo vận tốc: F = ma
	D3DXVECTOR2 normal; // nx, ny [ Để xác định hướng trái / phải, trên / dưới ]

	bool isDynamic; // phân biệt body tĩnh và body động (Mario, Goomba: dynamic) (Pipe, Block: k dynamic)
	bool isTrigger; // biến dùng để xét trả về OnCollisionEnter với OnTriggerEnter
public:
	CPhysicsBody();
	~CPhysicsBody();
	// Chỉ phát hiện va chạm thui còn việc xử lý sao thì tùy theo mỗi game object [ OnCollisionEnter]
	void PhysicsUpdate(LPCollisionBox cO, std::vector<LPCollisionBox>* coObjects);
	void Update(LPGameObject gO);

	bool CheckAABB(RectF selfBox, RectF otherBox);
	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny,
		GameObjectTags tag);

	LPCollisionEvent SweptAABBEx(LPCollisionBox cO, LPCollisionBox cOOther); // thay vì xét với các gameobject khác, thực chất mình chỉ xét với những collisionbox mà thôi
	void CalcPotentialCollisions(LPCollisionBox cO, std::vector<LPCollisionBox>* coObjects, std::vector<LPCollisionEvent>& coEvents);
	void FilterCollision(
		std::vector<LPCollisionEvent>& coEvents,
		std::vector<LPCollisionEvent>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny);

	D3DXVECTOR2 GetVelocity();
	void SetVelocity(D3DXVECTOR2 s);

	float GetGravity();
	void SetGravity(float gravity);

	float GetAcceleration();
	void SetAcceleration(float acc);

	D3DXVECTOR2 GetDragForce();
	void SetDragForce(D3DXVECTOR2 drag);

	D3DXVECTOR2 GetNormal();
	void SetNormal(D3DXVECTOR2 n);

	bool IsDynamic();
	void SetDynamic(bool isDynamic);

	D3DXVECTOR2 GetBounceForce();
	void SetBounceForce(D3DXVECTOR2 bF);
};



struct CollisionEvent
{
	LPCollisionBox obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPCollisionBox obj = NULL)
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
