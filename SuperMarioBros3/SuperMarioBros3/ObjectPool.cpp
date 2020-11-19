#include "ObjectPool.h"

void CObjectPool::Add(CGameObject* obj)
{
	pool.push_back(obj);
	obj->Enable(false);
}

CGameObject* CObjectPool::Init()
{
	if (IsEmpty() == true)
		return NULL;
	auto obj = pool.at(0);
	obj->Enable(true);
	pool.erase(pool.begin());
	return obj;
}

void CObjectPool::Revoke(CGameObject* obj)
{
	obj->Enable(false);
	pool.push_back(obj);
}

bool CObjectPool::IsEmpty()
{
	return pool.empty();
}

void CObjectPool::AddPoolToScene(CScene* scene)
{
	if (pool.size() == 0)
		return;
	for (auto x : pool)
		scene->AddObject(x);
}
