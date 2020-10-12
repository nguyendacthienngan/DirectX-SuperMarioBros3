#pragma once
#include <d3dx9.h>
class CCamera
{
private:
	float vx;
	D3DXVECTOR2 posCam;
	int widthCam;
	int heightCam;

	float boundaryLeft; // set biên camera dựa vào kích thước map: boundary.left = 0
	float boundaryRight; // VD: boundary.right =  map.width - cam.width => Tùy theo cách mình xét, đây chỉ là ví dụ
public:
	CCamera(int wid, int hei);
	~CCamera();
	void Update();
	D3DXVECTOR2 Transform(D3DXVECTOR2 posWorld);
	void CheckObjectInCamera(); //Kiểm tra xem obj truyền vào có nằm trong camera không để ta xử lý update / renderm

	int GetSpeedXCam();
	D3DXVECTOR2 GetPositionCam();
	int GetWidthCam();
	int GetHeightCam();
	float GetBoundaryLeft();
	float GetBoundaryRight();

	void SetSpeedXCam(float v);
	void SetPositionCam(D3DXVECTOR2 pos);
	void SetWidthCam(int w);
	void SetHeightCam(int h);
	void SetBoundary(float left, float right);

};

