#include "Camera.h"
#include "KeyboardManager.h"
#include "Game.h"

#include "SceneManager.h"
#include "Mario.h"
#include "GameObject.h"


#include <vector>
#include "Ultis.h"
using namespace std;
CCamera::CCamera(int wid, int hei, CMario* mario)
{
    widthCam = wid;
    heightCam = hei;
    boundaryLeft = 0;
    boundaryRight = 0;
    vx = 0.0f;
    
    player = mario;
}

CCamera::~CCamera()
{
    
}

void CCamera::Update()
{
    // xử lý khi người dùng ấn nút sẽ di chuyển theo nút ấn của người dùng
    this->dt = CGame::GetInstance()->GetFixedDeltaTime(); 
    
    //float playerX, playerY;
    //playerX = player->GetPosition().x + posCam.x; // đổi qua tọa độ thực?
    //playerY = player->GetPosition().y + posCam.y;
    //DebugOut(L"Position player: (x,y): (%f,%f) \n", playerX, playerY);

    //if (player != NULL)
    //{
    //    if (playerX > widthCam / 2 && playerX < widthCam)
    //    {
    //        // follow Mario
    //        //this->posCam.x += 100 * 0.02;
    //        this->posCam.x = playerX - widthCam/2 + 30;
    //    }
    //    //else
    //    //{
    //    //    //this->posCam.x -= 100 * 0.02;
    //    //    this->posCam.x = playerY - heightCam/2;
    //    //}
    //    
    //}

    auto keyManager = CKeyboardManager::GetInstance();

    if (keyManager->GetKeyDown(DIK_LEFT))
        this->posCam.x -= 300 * CGame::GetInstance()->GetFixedDeltaTime();

    if (keyManager->GetKeyDown(DIK_RIGHT))
        this->posCam.x += 300 * CGame::GetInstance()->GetFixedDeltaTime();

    if (keyManager->GetKeyDown(DIK_UP))
        this->posCam.y -= 300 * CGame::GetInstance()->GetFixedDeltaTime();

    if (keyManager->GetKeyDown(DIK_DOWN))
        this->posCam.y += 300 * CGame::GetInstance()->GetFixedDeltaTime();

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
