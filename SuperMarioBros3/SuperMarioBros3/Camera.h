#pragma once
#include <d3dx9.h>
#include "GameObject.h"
#include "RectF.h"
// là 1 object đặc biệt, có thể có nhiều camera (onGround, underGround)
class CGameObject;
typedef CGameObject* LPGameObject;

struct CameraPropertieSet;

class CCamera 
{
private:
	DWORD dt; // deltatime
	float vx;
	D3DXVECTOR2 posCam;
	float widthCam;
	float heightCam;

	LPGameObject gameObject; // lưu cái gameobject mà camera follow: Mario
	bool isDisablePosX;
	bool isDisablePosY;

	RectF currentBoundary;
	map<int, CameraPropertieSet> cameraPropertieSets;
public:
	CCamera(int wid, int hei);
	~CCamera();

	virtual void Update();
	virtual void Render();

	D3DXVECTOR2 Transform(D3DXVECTOR2 posWorld);
	D3DXVECTOR2 TransformCamToWorld(D3DXVECTOR2 posInCam);
	bool CheckObjectInCamera(D3DXVECTOR2 posObject, float widthObj, float heightObj); //Kiểm tra xem obj truyền vào có nằm trong camera không để ta xử lý update / renderm
	bool CheckRectInCamera(RECT rect);

	int GetSpeedXCam();
	void SetSpeedXCam(float v);

	D3DXVECTOR2 GetPositionCam();
	void SetPositionCam(D3DXVECTOR2 pos);

	float GetWidthCam();
	void SetWidthCam(float w);

	float GetHeightCam();
	void SetHeightCam(float h);

	RectF GetCurrentBoundary();
	void SetCurrentBoundary(RectF bound);

	CameraPropertieSet GetCameraProperties(int id);
	void AddCameraProperties(int id, D3DXVECTOR2 pos, RectF boundary);
	void AddCameraProperties(int id, CameraPropertieSet camProps);

	LPGameObject GetGameObject();
	void SetGameObject(LPGameObject gO);

	bool GetDisablePosX();
	void SetDisablePosX(bool isDisableX);

	bool GetDisablePosY();
	void SetDisablePosY(bool isDisableY);
};

struct CameraPropertieSet
{
	D3DXVECTOR2 camPosition; // tọa độ trái trên
	RectF boundarySet;
	static CameraPropertieSet Empty()
	{
		D3DXVECTOR2 pos(-1, -1);
		RectF rect;
		rect.left = -1;
		rect.right = -1;
		rect.top = -1;
		rect.bottom = -1;
		return CameraPropertieSet{ pos, rect };
	}
};