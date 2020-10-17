#pragma once
//#include "GameObject.h"
#include "CollisionBox.h"
#include <d3d9.h>
#include <d3d9.h>

struct CCollisionEvent;
typedef CCollisionEvent* LPCollisionEvent;

// PhysicBody đảm nhiệm vụ mô phỏng chuyển động VẬT LÝ của Game Object => Xử lý va chạm
// Chứ không dùng để dụng chạm với thằng nào hết => Chuyện đó của CollisionBox
// Khi áp dụng PhysicBody lên vật nào thì vật đó sẽ có va chạm với vật khác nếu vật đó có CollisionBox

// VD: Mario = Body + CollisionBox, Ghost = Body, Pipe = CollisionBox

// Lưu ý Body khác với Transform, Transform chỉ là phép biến hình, không ràng buộc bởi trọng lực, khối lượng, vận tốc
// Body sẽ thay đổi Transform nhưng không ngược lại

//class CGameObject;
//typedef CGameObject* LPGameObject;

class CCollisionBox;
typedef CCollisionBox* LPCCollisionBox;

class CPhysicsBody
{
private:
	D3DXVECTOR2 velocity;
	float gravity;
	bool isDynamic; // phân biệt body tĩnh và body động (Mario, Goomba: dynamic) (Pipe, Block: k dynamic)

public:

	void PhysicsUpdate(std::vector<CCollisionBox>* coObjects);

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

	LPCollisionEvent SweptAABBEx(LPCCollisionBox cO, LPCCollisionBox cOOther); // thay vì xét với các gameobject khác, thực chất mình chỉ xét với những collisionbox mà thôi
	void CalcPotentialCollisions(LPCCollisionBox cO, std::vector<LPCCollisionBox>* coObjects, std::vector<LPCollisionEvent>& coEvents);
	void FilterCollision(
		std::vector<LPCollisionEvent>& coEvents,
		std::vector<LPCollisionEvent>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);


	D3DXVECTOR2 GetSpeed();

	void SetDynamic(bool isDynamic);
	void SetGravity(float gravity);
};



struct CCollisionEvent
{
	LPCCollisionBox obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPCCollisionBox obj = NULL)
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
