#include "PhysicsBody.h"
#include "Game.h"

#include <vector>
#include <minwindef.h>
#include <algorithm>
#include "Ultis.h"
#include "Enemy.h"

using namespace std;

CPhysicsBody::CPhysicsBody()
{
	isTrigger = false;
	velocity.x = 0;
	velocity.y = 0;
	gravity = 0;
	acceleration = 0;
	dragForce.x = 0;
	dragForce.y = 0;
	normal.x = 1;
	normal.y = 1;
	bounceForce.x = 0;
	bounceForce.y = 0;
}

CPhysicsBody::~CPhysicsBody()
{
}

void CPhysicsBody::PhysicsUpdate(LPCollisionBox cO, std::vector<LPCollisionBox>* coObjects)
{
	auto gameObject = cO->GetGameObjectAttach();
	auto collisionBox = gameObject->GetCollisionBox()->at(0);

	if (gameObject == NULL || gameObject->IsEnabled() == false || isDynamic == false)
		return;

	
	auto dt = CGame::GetInstance()->GetDeltaTime();
	auto pos = gameObject->GetPosition();
	auto distance = collisionBox->GetDistance();
	vector<CollisionEvent*> coEvents;
	vector<CollisionEvent*> coEventsResult;

	velocity.y += gravity * dt;

	coEvents.clear();

	CalcPotentialCollisions(cO, coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		pos.x += distance.x;
		pos.y += distance.y;
		gameObject->SetPosition(pos);
	}
	else
	{
		// Collision detetion
		float min_tx, min_ty, nx = 0, ny; 

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		pos = gameObject->GetPosition(); 

		// block every object first!
		if (isTrigger == false)
		{
			pos.x += min_tx * distance.x + nx *0.4f; 
			pos.y += min_ty * distance.y + ny *0.4f;

			cO->CollisionHandle(dt, coEvents, this, velocity, min_tx, min_ty, nx, ny);
		}
		
		if (nx != 0 || ny != 0)
		{
			if (isTrigger == true)
				gameObject->OnTriggerEnter(cO, coEventsResult);
			else
				gameObject->OnCollisionEnter(cO, coEventsResult);
		}
		gameObject->SetPosition(pos);
	}

	for (unsigned i = 0; i < coEvents.size(); i++) delete coEvents[i];
	coEvents.clear();

}
void CPhysicsBody::Update(LPGameObject gameObject)
{
	if (gameObject == NULL || gameObject->IsEnabled() == false)
		return;
	auto dt = CGame::GetInstance()->GetDeltaTime();
	auto collisionBoxs = gameObject->GetCollisionBox();
	auto physiscBody = gameObject->GetPhysiscBody();
	
	D3DXVECTOR2 distance;
	distance.x = physiscBody->GetVelocity().x * dt;
	distance.y = physiscBody->GetVelocity().y * dt;
	collisionBoxs->at(0)->SetDistance(distance);
}

bool CPhysicsBody::CheckAABB(RectF selfBox, RectF otherBox)
{
	return 
		( (selfBox.left <= otherBox.right)	&& (selfBox.right >= otherBox.left) && (selfBox.top <= otherBox.bottom) && (selfBox.bottom >= otherBox.top) // A overlaps B
		|| (selfBox.left <= otherBox.left) && (selfBox.right >= otherBox.right) && (selfBox.top <= otherBox.top) && (selfBox.bottom >= otherBox.bottom)); // A contains B
}
/*
	Standard sweptAABB implementation
	Source: GameDev.net
*/
void CPhysicsBody::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny,
	GameObjectTags tag)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

#pragma region Broad-phase Test

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

#pragma endregion

#pragma region Sweep AABB Test
	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -999999.0f;
		tx_exit = 999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999.0f;
		ty_exit = 99999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		tag == GameObjectTags::GhostPlatform ? nx = 0.0f : (dx > 0 ? nx = -1.0f : nx = 1.0f);
	}
	else
	{
		nx = 0.0f;
		// Ghost Platform thì mình k xét ở dưới lên hoặc va chạm ngang
		dy > 0 ? ny = -1.0f : (tag == GameObjectTags::GhostPlatform ? ny = 0.0f : ny = 1.0f);
	}
#pragma endregion
}

/*
	Extension of original SweptAABB to deal with two moving objects: A đang chạy và kiểm tra coi có với va chạm với B đang dứng yên
*/
LPCollisionEvent CPhysicsBody::SweptAABBEx(LPCollisionBox cO, LPCollisionBox cOOther)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;
	

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	auto phyBody = cOOther->GetGameObjectAttach()->GetPhysiscBody();

	svx = phyBody->GetVelocity().x;
	svy = phyBody->GetVelocity().y;
	float sdx = svx * CGame::GetInstance()->GetDeltaTime(); // sdx = svx * dt
	float sdy = svy * CGame::GetInstance()->GetDeltaTime();

	// deal with moving object: m speed = original m speed - collide object speed
	float dx = cO->GetDistance().x - sdx; // laasy vaan toc cua minh tru van toc cua thang dang chay => Xet giua 1 dang chay (minh) va 1 dung yen => Theo dinh ly Newton
	float dy = cO->GetDistance().y - sdy;

	auto boundingBox = cO->GetBoundingBox(); // A
	ml = boundingBox.left;
	mt = boundingBox.top;
	mr = boundingBox.right;
	mb = boundingBox.bottom;

	auto boundingBoxOther = cOOther->GetBoundingBox(); // B
	sl = boundingBoxOther.left;
	st = boundingBoxOther.top;
	sr = boundingBoxOther.right;
	sb = boundingBoxOther.bottom;

	std::string name = cOOther->GetName();
	SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny, cOOther->GetGameObjectAttach()->GetTag()
	);

	CollisionEvent* e = new CollisionEvent(t, nx, ny, dx, dy, cOOther);
	return e;
}


void CPhysicsBody::CalcPotentialCollisions(
	LPCollisionBox cO,
	std::vector<LPCollisionBox>* coObjects,
	std::vector<LPCollisionEvent>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (cO->IsEnabled() == false || coObjects->at(i)->IsEnabled() == false)
			continue;
		// Trừ solidbox ra thì tất cả gameobject bị disable thì k được xét va chạm vật lý
		if (coObjects->at(i)->GetGameObjectAttach()->IsEnabled() == false && (coObjects->at(i)->GetGameObjectAttach()->GetTag() != GameObjectTags::Solid))
			continue;
		if (coObjects->at(i) == cO)
			continue;

		if (cO->GetGameObjectAttach()->CanCollisionWithThisObject(coObjects->at(i)->GetGameObjectAttach(), coObjects->at(i)->GetGameObjectAttach()->GetTag())
			== false)
			continue;
		// Có overlap (Dùng AABB)
		if (coObjects->at(i)->GetGameObjectAttach()->GetTag() != GameObjectTags::Solid && cO->GetGameObjectAttach()->GetTag() != GameObjectTags::Solid)
		{
			if (CheckAABB(cO->GetBoundingBox(), coObjects->at(i)->GetBoundingBox()) == true || CheckAABB(coObjects->at(i)->GetBoundingBox(), cO->GetBoundingBox()) == true)
			{
				cO->GetGameObjectAttach()->OnOverlappedEnter(cO, coObjects->at(i));
				coObjects->at(i)->GetGameObjectAttach()->OnOverlappedEnter(coObjects->at(i), cO);
				continue;
			}
		}
		LPCollisionEvent e = SweptAABBEx(cO,coObjects->at(i));
		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
			std::string name = coObjects->at(i)->GetName();
			
			//OutputDebugString(ToLPCWSTR("Hit Name: " + name + "\n"));
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);
}


// Tìm ra thằng va chạm đầu tiên trong 1 loạt các thằng có thể va chạm
void CPhysicsBody::FilterCollision(
	vector<LPCollisionEvent>& coEvents,
	vector<LPCollisionEvent>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;	
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCollisionEvent c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; 
			nx = c->nx; 
			min_ix = i; 
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; 
			ny = c->ny; 
			min_iy = i; 
		}
	}

	// Cách cũ của thầy: Chưa xét ghost platform (Xét 4 hướng)
	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);

}

D3DXVECTOR2 CPhysicsBody::GetVelocity()
{
	return velocity;
}

float CPhysicsBody::GetGravity()
{
	return gravity;
}

float CPhysicsBody::GetAcceleration()
{
	return acceleration;
}

D3DXVECTOR2 CPhysicsBody::GetDragForce()
{
	return dragForce;
}

D3DXVECTOR2 CPhysicsBody::GetNormal()
{
	return normal;
}

bool CPhysicsBody::IsDynamic()
{
	return isDynamic;
}

void CPhysicsBody::SetVelocity(D3DXVECTOR2 s)
{
	velocity = s;
}

void CPhysicsBody::SetDynamic(bool isDynamic)
{
	this->isDynamic = isDynamic;
}

bool CPhysicsBody::IsTrigger()
{
	return isTrigger;
}

void CPhysicsBody::SetTrigger(bool isTrigg)
{
	this->isTrigger = isTrigg;
}

D3DXVECTOR2 CPhysicsBody::GetBounceForce()
{
	return bounceForce;
}

void CPhysicsBody::SetBounceForce(D3DXVECTOR2 bF)
{
	bounceForce = bF;
}

void CPhysicsBody::SetGravity(float gravity)
{
	this->gravity = gravity;
}

void CPhysicsBody::SetAcceleration(float acc)
{
	acceleration = acc;
}

void CPhysicsBody::SetDragForce(D3DXVECTOR2 drag)
{
	dragForce = drag;
}

void CPhysicsBody::SetNormal(D3DXVECTOR2 n)
{
	this->normal = n;
}
