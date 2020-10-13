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

void CCamera::Update(DWORD dt)
{
    // xử lý khi người dùng ấn nút sẽ di chuyển theo nút ấn của người dùng

    this->dt = dt; 
    
    if (posCam.x < boundaryLeft)
        posCam.x = boundaryLeft;

    if (posCam.x > boundaryRight)
        posCam.x = boundaryRight;
}

void CCamera::Render()
{
}

D3DXVECTOR2 CCamera::Transform(D3DXVECTOR2 posWorld)
{
    return D3DXVECTOR2(posWorld.x - posCam.x, posWorld.y - posCam.y);
}

bool CCamera::CheckObjectInCamera(D3DXVECTOR2 posObject, float widthObj, float heightObj) // Thông tin của object
{
    if (posObject.x + widthObj < posCam.x || posObject.x > posCam.x + widthCam)
        return false;
    if (posObject.y + heightObj < posCam.y || posObject.y > posCam.y + heightCam)
        return false;
    return true; // Object nằm trong Cam
}

bool CCamera::CheckRectInCamera(RECT rect)
{
    auto width = rect.right - rect.left;
    auto height = rect.top - rect.bottom;

    /*return rect.left >= posCam.x - width && rect.right <= posCam.x + widthCam + width &&
        rect.top >= posCam.y - height && rect.bottom <= posCam.y + heightCam + height;*/

   // return false;

    if (rect.left + width < posCam.x || rect.left > posCam.x + widthCam)
        return false;
    if (rect.top + height < posCam.y || rect.top > posCam.y + heightCam)
        return false;
    return true;
}

int CCamera::GetSpeedXCam()
{
    return vx;
}

D3DXVECTOR2 CCamera::GetPositionCam()
{
    return posCam;
}

float CCamera::GetWidthCam()
{
    return widthCam;
}

float CCamera::GetHeightCam()
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

void CCamera::SetWidthCam(float w)
{
    widthCam = w;
}

void CCamera::SetHeightCam(float h)
{
    heightCam = h;
}

void CCamera::SetBoundary(float left, float right)
{
    boundaryLeft = left;
    boundaryRight = right;
}
