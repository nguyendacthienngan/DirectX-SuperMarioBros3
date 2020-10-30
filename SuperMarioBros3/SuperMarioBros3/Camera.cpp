#include "Camera.h"
#include "KeyboardManager.h"
#include "Game.h"

#include "SceneManager.h"
#include "Mario.h"

#include "MarioConst.h"

#include <vector>
#include "Ultis.h"
#include "Const.h"
using namespace std;
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
    this->dt = CGame::GetInstance()->GetFixedDeltaTime(); 
    
    float x, y;
    x = gameObject->GetPosition().x;
    y = gameObject->GetPosition().y;
    
     // follow Mario
    posCam.x = x - widthCam * 0.5f;
    posCam.y = y - widthCam * 0.5f;

    // Ở đầu scene và cuối scene ta sẽ đặt ra boundary => Mario k được vượt quá boundary này
    if (posCam.x < boundaryLeft)
        posCam.x = boundaryLeft;

    if (posCam.x > boundaryRight - widthCam)
        posCam.x = boundaryRight - widthCam;

    if (posCam.y < boundaryTop)
        posCam.y = boundaryTop;

    if (posCam.y > boundaryBottom - heightCam)
        posCam.y = boundaryBottom - heightCam;
    
    //	Xét biên để chỉnh lại camera k thoát khỏi camera
    if (x > boundaryRight + widthCam - 24)
        x = boundaryRight + widthCam - 24;
    else if (x < boundaryLeft + 24)
        x = boundaryLeft + 24;
    gameObject->SetPosition(D3DXVECTOR2(x, y));

}

void CCamera::Render()
{
}



D3DXVECTOR2 CCamera::Transform(D3DXVECTOR2 posWorld)
{
    return D3DXVECTOR2(posWorld.x - posCam.x, posWorld.y - posCam.y);
}

D3DXVECTOR2 CCamera::TransformCamToWorld(D3DXVECTOR2 posInCam)
{
    return D3DXVECTOR2(posInCam.x + posCam.x, posInCam.y + posCam.y);
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
    auto rectWidth = rect.right - rect.left;
    auto rectHeight = rect.bottom - rect.top;

    return rect.left >= posCam.x - rectWidth && rect.right <= posCam.x + widthCam + rectWidth &&
        rect.top >= posCam.y - rectHeight && rect.bottom <= posCam.y + heightCam + rectHeight;

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

RectF CCamera::GetBoundary()
{
    RectF r;
    r.left = boundaryLeft;
    r.right = boundaryRight;
    r.top = boundaryTop;
    r.bottom = boundaryBottom;
    return RectF(r);
}

LPGameObject CCamera::GetGameObject()
{
    return gameObject;
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

void CCamera::SetBoundary(float left, float right, float top, float bottom)
{
    boundaryLeft = left;
    boundaryRight = right;
    boundaryTop= top;
    boundaryBottom = bottom;
}

void CCamera::SetGameObject(LPGameObject gO)
{
    gameObject = gO;
}


