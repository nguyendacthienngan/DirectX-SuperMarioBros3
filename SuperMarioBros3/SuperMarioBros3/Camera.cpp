#include "Camera.h"
#include "KeyboardManager.h"
#include "Game.h"

#include "SceneManager.h"
#include "Mario.h"

#include "MarioConst.h"
#include "CameraConst.h"

#include <vector>
#include <map>
#include "Ultis.h"
#include "Const.h"
using namespace std;
CCamera::CCamera()
{
}
CCamera::CCamera(int wid, int hei)
{
    widthCam = wid;
    heightCam = hei;
    vx = 0.0f;
    isDisablePosX = false;
    isDisablePosY = false;
    isAutoX = false;
}

CCamera::~CCamera()
{
    
}

void CCamera::Update()
{
    if (gameObject == NULL)
        return;
    this->dt = CGame::GetInstance()->GetFixedDeltaTime() * CGame::GetTimeScale();
    float x, y;
    x = gameObject->GetPosition().x;
    y = gameObject->GetPosition().y;
    if (isAutoX == true)
    {
        posCam.x += CAMERA_VEL_X * dt;

        if (x < posCam.x + 24)
        {
            x = posCam.x + 24;
            auto mario = static_cast<CMario*>(gameObject);
            MarioStateSet state;
            state.move = MoveOnGroundStates::Walk;
            state.jump = JumpOnAirStates::Stand;
            mario->SetCurrentPhysicsState(state);
        }
    }
    else
    {
        if (isDisablePosX == false)
        {
            // follow Mario
            posCam.x = x - widthCam * 0.5f;

            if (isDisablePosY == false)
                posCam.y = y - heightCam * 0.1f;


            //	Xét biên để chỉnh lại camera k thoát khỏi camera
            if (x > currentBoundary.right - 24 && isDisablePosX == false)
                x = currentBoundary.right - 24;
            else if (x < currentBoundary.left + 24 && isDisablePosX == false)
                x = currentBoundary.left + 24;
        }
       
    }
    // Ở đầu scene và cuối scene ta sẽ đặt ra boundary => Mario k được vượt quá boundary này
    if (posCam.x < currentBoundary.left)
        posCam.x = currentBoundary.left;

    if (posCam.x > currentBoundary.right - widthCam)
        posCam.x = currentBoundary.right - widthCam;

    if (posCam.y < currentBoundary.top)
        posCam.y = currentBoundary.top;

    if (posCam.y > currentBoundary.bottom - heightCam)
        posCam.y = currentBoundary.bottom - heightCam;

    if (isDisablePosY == false)
        if (posCam.y < currentBoundary.bottom - heightCam && y >= currentBoundary.bottom - heightCam * 0.75)
            posCam.y = currentBoundary.bottom - heightCam;

    if (y > posCam.y + heightCam || y > currentBoundary.bottom)
    {
        auto mario = static_cast<CMario*>(gameObject);
        mario->OnDie();
    }
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

bool CCamera::CheckObjectInCamera(LPGameObject gO) // Thông tin của object
{
    if (gO->GetTag() == GameObjectTags::PlayerController || gO->GetTag() == GameObjectTags::Effect)
        return true;
    if (gO->MarioTag(gO->GetTag()))
        return true;
    D3DXVECTOR2 posObject = gO->GetPosition();
    auto size = CGameObject::GetGameObjectSize(gO);
    auto translate = 48 * 6;

    if (gO->GetTag() == GameObjectTags::Solid || gO->GetTag() == GameObjectTags::GhostPlatform)
    {
        posObject -= size * 0.5f;
    }
    if (gO->GetTag() == GameObjectTags::MovingPlatform)
        translate = 150;

    if (posObject.x + size.x + translate < posCam.x || posObject.x > posCam.x + widthCam + translate)
        return false;
    if (posObject.y + size.y + translate < posCam.y || posObject.y > posCam.y + heightCam + translate)
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

RectF CCamera::GetCurrentBoundary()
{
    return currentBoundary;
}

LPGameObject CCamera::GetGameObject()
{
    return gameObject;
}

bool CCamera::GetDisablePosX()
{
    return isDisablePosX;
}

bool CCamera::GetDisablePosY()
{
    return isDisablePosY;
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

void CCamera::SetCurrentBoundary(RectF bound)
{
    currentBoundary = bound;
}

CameraPropertieSet CCamera::GetCameraProperties(int id)
{
    if (cameraPropertieSets.find(id) != cameraPropertieSets.end())
        return cameraPropertieSets.at(id);
    return CameraPropertieSet::Empty();
}

void CCamera::AddCameraProperties(int id, D3DXVECTOR2 pos, RectF boundary, bool isDisableX, bool isDiableY)
{
    this->cameraPropertieSets.insert(make_pair(id, CameraPropertieSet{ pos, boundary, isDisableX, isDiableY }));
}

void CCamera::AddCameraProperties(int id, CameraPropertieSet camProps)
{
    this->cameraPropertieSets.insert(make_pair(id, camProps));

}

void CCamera::SetGameObject(LPGameObject gO)
{
    gameObject = gO;
}

void CCamera::SetDisablePosX(bool isDisableX)
{
    this->isDisablePosX = isDisableX;
}

void CCamera::SetDisablePosY(bool isDisableY)
{
    this->isDisablePosY = isDisableY;
}

bool CCamera::GetAutoX()
{
    return isAutoX;
}

void CCamera::SetAutoX(bool isAutoX)
{
    this->isAutoX = isAutoX;
}


