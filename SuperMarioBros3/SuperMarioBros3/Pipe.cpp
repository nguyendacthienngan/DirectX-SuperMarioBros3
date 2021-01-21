#include "Pipe.h"
#include "SpriteManager.h"

CPipe::CPipe(D3DXVECTOR2 size)
{
	tag = GameObjectTags::Pipe;
	this->pipeSize = size;
	pipeSize.y = size.y / PIPE_DEFAULT_TILE_SIZE;
	pipeSize.x = size.x / PIPE_DEFAULT_TILE_SIZE;


	pipeSprite = CSpriteManager::GetInstance()->Get("spr-pipe-0");
	this->physiscBody->SetDynamic(false);
	isEnabled = true;
}

void CPipe::Render(CCamera* cam, int alpha)
{
	int size = PIPE_DEFAULT_TILE_SIZE;
	D3DXVECTOR2 pos = transform.position;
	auto translation = -cam->GetPositionCam() + DEFAULT_OFFSET;
	auto headOffset = pipeDirection == PipeDirection::Up ? 0 : (pipeSize.y - 1) * size;


	pipeSprite->SetRect(headRect[0]);
	pipeSprite->Draw(D3DXVECTOR2(pos.x + translation.x, pos.y + headOffset + translation.y), D3DXVECTOR2(1,1), 0);

	pipeSprite->SetRect(headRect[1]);
	pipeSprite->Draw(D3DXVECTOR2(pos.x + size + translation.x, pos.y + headOffset + translation.y), D3DXVECTOR2(1, 1), 0);

	int startY = pipeDirection == PipeDirection::Up ? 1 : 0;
	int endY = pipeDirection == PipeDirection::Down ? pipeSize.y - 1 : pipeSize.y;
	for (int y = startY; y < endY; ++y)
	{
		pipeSprite->SetRect(bodyRect[0]);
		pipeSprite->Draw(D3DXVECTOR2(pos.x + translation.x, pos.y + size * y + translation.y), D3DXVECTOR2(1, 1), 0);
		pipeSprite->SetRect(bodyRect[1]);
		pipeSprite->Draw(D3DXVECTOR2(pos.x + size + translation.x, pos.y + size * y + translation.y), D3DXVECTOR2(1, 1), 0);
	}
}

void CPipe::SetPipeSprite(CSprite* pipeSprite)
{
	this->pipeSprite = pipeSprite;
}

void CPipe::SetHeadRect(RECT headRect1, RECT headRect2)
{
	headRect[0] = headRect1;
	headRect[1] = headRect2;
}

void CPipe::SetBodyRect(RECT bodyRect1, RECT bodyRect2)
{
	bodyRect[0] = bodyRect1;
	bodyRect[1] = bodyRect2;
}

void CPipe::SetDirection(PipeDirection pipeDirection)
{
	this->pipeDirection = pipeDirection;
}

bool CPipe::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	return false;
}
