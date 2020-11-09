#include "stdafx.h"
#include "ObjectPool.h"
#include "GameObject.h"
#include "StatsComponent.h"

CObjectPool* CObjectPool::ourInstance = nullptr;

CObjectPool* CObjectPool::GetInstance()
{
	return ourInstance;
}

CObjectPool::CObjectPool()
{
	ourInstance = this;
	firstAvailable = &enemies[0];

	for (int i = 0; i < poolSize - 1; ++i) {
		enemies[i].SetNext(&enemies[i + 1]);
	}

	enemies[poolSize - 1].SetNext(NULL);
}

CObjectPool::~CObjectPool()
{
	ourInstance = nullptr;
}

CGameObject* CObjectPool::Create(DirectX::SimpleMath::Vector3 aPosition, float aHealth, float aDamage, float aMoveSpeed, float aCooldown)
{
	assert(firstAvailable != NULL);

	CEnemy* newEnemy = firstAvailable;
	firstAvailable = newEnemy->GetNext();

	newEnemy->init(aPosition, aHealth, aDamage, aMoveSpeed, aCooldown);
	return newEnemy->GetEnemy();
}

void CObjectPool::Remove(CGameObject* aEnemy)
{
	for (int i = 0; i < poolSize; ++i) {
		if (enemies[i].GetEnemy() == aEnemy) {
			enemies[i].GetEnemy()->SetActive(false);
			enemies[i].SetNext(firstAvailable);
			firstAvailable = &enemies[i];
		}
	}
}
