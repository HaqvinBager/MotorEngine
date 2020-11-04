#include "stdafx.h"
#include "CollisionManager.h"
#include "IntersectionManager.h"
#include "CapsuleColliderComponent.h"
//#include "TriangleColliderComponent.h"
#include "TransformComponent.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

CCollisionManager* CCollisionManager::ourInstance = nullptr;
CCollisionManager* CCollisionManager::GetInstance()
{
    return ourInstance;
}

CCollisionManager::CCollisionManager()
{
    ourInstance = this;
}

CCollisionManager::~CCollisionManager()
{
    ourInstance = nullptr;
}

void CCollisionManager::RegisterCollider(CCapsuleColliderComponent* aCollider)
{
    assert(aCollider != nullptr && "Collider is nullptr");
    myCapsuleColliders.emplace_back(aCollider);
}

//void CCollisionManager::RegisterCollider(CTriangleColliderComponent* aCollider)
//{
//    assert(aCollider != nullptr && "Collider is nullptr");
//    myCapsuleColliders.emplace_back(aCollider);
//}

void CCollisionManager::Update()
{
    for (unsigned int outer = 0; outer < myCapsuleColliders.size(); ++outer)
    {
        for (unsigned int inner = outer + 1; inner < myCapsuleColliders.size(); ++inner)
        {
            if (CIntersectionManager::CapsuleIntersection(*myCapsuleColliders[outer], *myCapsuleColliders[inner]))
            {
                myCapsuleColliders[outer]->GetParent().Collided(myCapsuleColliders[inner]->GetParent());
                myCapsuleColliders[inner]->GetParent().Collided(myCapsuleColliders[outer]->GetParent());
            }
        }
    }
}