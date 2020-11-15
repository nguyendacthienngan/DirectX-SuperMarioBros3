#include "Block.h"
#include "BlockConst.h"
CBlock::CBlock()
{
	Init();
}
void CBlock::Init()
{
	tag = GameObjectTags::Solid;
	CCollisionBox* box = new CCollisionBox();
	box->SetSizeBox(BLOCK_BBOX);
	box->SetGameObjectAttach(this);
	this->collisionBoxs->push_back(box);
	this->physiscBody->SetDynamic(false);
}
