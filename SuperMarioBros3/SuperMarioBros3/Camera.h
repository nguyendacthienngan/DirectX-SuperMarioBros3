#pragma once
#include <d3dx9.h>

// là 1 object đặc biệt, có thể có nhiều camera (onGround, underGround)
class CCamera 
{
private:
	DWORD dt; // deltatime
	float vx;
	D3DXVECTOR2 posCam;
	float widthCam;
	float heightCam;

	float boundaryLeft; // set biên camera dựa vào kích thước map: boundary.left = 0
	float boundaryRight; // VD: boundary.right =  map.width - cam.width => Tùy theo cách mình xét, đây chỉ là ví dụ
public:
	CCamera(int wid, int hei);
	~CCamera();

	virtual void Update(DWORD dt);
	virtual void Render();

	D3DXVECTOR2 Transform(D3DXVECTOR2 posWorld);
	bool CheckObjectInCamera(D3DXVECTOR2 posObject, float widthObj, float heightObj); //Kiểm tra xem obj truyền vào có nằm trong camera không để ta xử lý update / renderm
	bool CheckRectInCamera(RECT rect);

	int GetSpeedXCam();
	D3DXVECTOR2 GetPositionCam();
	float GetWidthCam();
	float GetHeightCam();
	float GetBoundaryLeft();
	float GetBoundaryRight();

	void SetSpeedXCam(float v);
	void SetPositionCam(D3DXVECTOR2 pos);
	void SetWidthCam(float w);
	void SetHeightCam(float h);
	void SetBoundary(float left, float right);

};

