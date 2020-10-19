#pragma once
//#include "GameObject.h"
#include "CollisionBox.h"
#include <d3d9.h>
#include <d3d9.h>

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
	D3DXVECTOR2 velocity;
	float gravity;
	bool isDynamic; // phân biệt body tĩnh và body động (Mario, Goomba: dynamic) (Pipe, Block: k dynamic)
	bool isTrigger; // biến dùng để xét trả về OnCollisionEnter với OnTriggerEnter
public:
	CPhysicsBody();
	// Chỉ phát lý va chạm thui còn việc xử lý sao thì tùy theo mỗi game object
	void PhysicsUpdate(LPCollisionBox cO, std::vector<LPCollisionBox>* coObjects);
	void Update(LPGameObject gO);

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
		float& ny);

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
	bool IsDynamic();

	void SetVelocity(D3DXVECTOR2 s);
	void SetDynamic(bool isDynamic);
	void SetGravity(float gravity);
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
