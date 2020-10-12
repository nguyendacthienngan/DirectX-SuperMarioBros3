#include "Camera.h"

CCamera::CCamera(int wid, int hei)
{
    widthCam = wid;
    heightCam = hei;
    boundaryLeft = 0;
    boundaryRight = 0;
    vx = 0.0f;
}

CCamera::~CCamera()
{
}

void CCamera::Update()
{
}

D3DXVECTOR2 CCamera::Transform(D3DXVECTOR2 posWorld)
{
    return D3DXVECTOR2(posWorld.x - posCam.x, posWorld.y - posCam.y);
}

void CCamera::CheckObjectInCamera()
{
}

int CCamera::GetSpeedXCam()
{
    return vx;
}

D3DXVECTOR2 CCamera::GetPositionCam()
{
    return posCam;
}

int CCamera::GetWidthCam()
{
    return widthCam;
}

int CCamera::GetHeightCam()
{
    return heightCam;
}

float CCamera::GetBoundaryLeft()
{
    return boundaryLeft;
}

float CCamera::GetBoundaryRight()
{
    return boundaryRight;
}

void CCamera::SetSpeedXCam(float v)
{
    vx = v;
}

void CCamera::SetPositionCam(D3DXVECTOR2 pos)
{
    posCam.x = pos.x;
    posCam.y = pos.y;
}

void CCamera::SetWidthCam(int w)
{
    widthCam = w;
}

void CCamera::SetHeightCam(int h)
{
    heightCam = h;
}

void CCamera::SetBoundary(float left, float right)
{
    boundaryLeft = left;
    boundaryRight = right;
}
