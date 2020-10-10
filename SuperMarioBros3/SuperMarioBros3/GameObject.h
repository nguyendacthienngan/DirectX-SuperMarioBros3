#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Animation.h"


class CGameObject;
typedef CGameObject* LPGameObject;
class CGameObject
{
protected:
	int id;
	DWORD dt;

	D3DXVECTOR2 position;
	D3DXVECTOR2 velocity;
	D3DXVECTOR2 distance; // dx = vx*dt, dy = vy*dt, độ dời sau một khoảng thời gian
	D3DXVECTOR2 normal; // vector pháp tuyến? nx, ny

	//STATE?
	std::string currentState;

	bool isEnabled;

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

	virtual void OnKeyDown(int KeyCode) = 0; // sẽ thuần ảo để đến từng gameObject cụ thể sẽ tự xử lý
	virtual void OnKeyUp(int KeyCode) = 0;

	bool IsEnabled();

	void SetPosition(D3DXVECTOR2 p) { this->position = p; }
	void SetSpeed(D3DXVECTOR2 v) { this->velocity = v; }
	D3DXVECTOR2 GetPosition() { return position; }
	D3DXVECTOR2 GetSpeed() { return velocity; }
	void SetState(std::string state);

};

#endif