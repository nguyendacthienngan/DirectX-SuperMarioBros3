#include "Camera.h"
#include "KeyboardManager.h"
#include "Game.h"

#include "SceneManager.h"
#include "Mario.h"
#include "GameObject.h"

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
    
   /* player = mario;
    player->SetBoundary(D3DXVECTOR2(0, 800));*/

}

CCamera::~CCamera()
{
    
}

void CCamera::Update()
{
    this->dt = CGame::GetInstance()->GetFixedDeltaTime(); 
    
    float x, y, vx, vy;
    x = posMario.x;  // tọa độ theo hệ quy chiếu Camera
    y = posMario.y;
    vx = speedMario.x;
    vy = speedMario.y;
     // follow Mario



    //  Phải luôn đem nhân vật chính vào chính giữa khi nhân vật đi vào giữa khung hình!!!

    if (x > SCREEN_WIDTH / 2 - 30 && x < SCREEN_WIDTH / 2 + 30)
    {
        if (vx > 0)
            this->posCam.x += MARIO_WALKING_SPEED * 20;
        else if (vx < 0)
            this->posCam.x -= MARIO_WALKING_SPEED * 20;
    }

    if (vx < 0)
        this->posCam.x -= MARIO_WALKING_SPEED * 20;
    

    if (posCam.x < boundaryLeft)
        posCam.x = boundaryLeft;

    if (posCam.x > boundaryRight)
        posCam.x = boundaryRight;

    // Khi bắt đầu màn, mario ở bên trái cùng
    // Set boundary left right là 0 và 250

    // Khi kết màn boundary left right là 350 và 600?

    /*auto keyManager = CKeyboardManager::GetInstance();

    if (keyManager->GetKeyDown(DIK_LEFT))
        this->posCam.x -= 300 * CGame::GetInstance()->GetFixedDeltaTime();

    if (keyManager->GetKeyDown(DIK_RIGHT))
        this->posCam.x += 300 * CGame::GetInstance()->GetFixedDeltaTime();

    if (keyManager->GetKeyDown(DIK_UP))
        this->posCam.y -= 300 * CGame::GetInstance()->GetFixedDeltaTime();

    if (keyManager->GetKeyDown(DIK_DOWN))
        this->posCam.y += 300 * CGame::GetInstance()->GetFixedDeltaTime();*/
        

   
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

D3DXVECTOR2 CCamera::GetPositionMario()
{
    return posMario;
}

D3DXVECTOR2 CCamera::GetSpeedMario()
{
    return speedMario;
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

void CCamera::SetPositionMario(D3DXVECTOR2 posMario)
{
    this->posMario = posMario;
}

void CCamera::SetSpeedMario(D3DXVECTOR2 speedMario)
{
    this->speedMario = speedMario;
}
