#include "stdafx.h"
#include "CollisionManager.h"
#include "IntersectionManager.h"
//#include "CapsuleColliderComponent.h"
//#include "TriangleColliderComponent.h"
//#include "CircleColliderComponent.h"
//#include "RectangleColliderComponent.h"
#include "Collider.h"
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

//void CCollisionManager::RegisterCollider(CCapsuleColliderComponent* aCollider)
//{
//    assert(aCollider != nullptr && "Collider is nullptr");
//    myCapsuleColliders.emplace_back(aCollider);
//}
//
//void CCollisionManager::RegisterCollider(CTriangleColliderComponent* aCollider)
//{
//    assert(aCollider != nullptr && "Collider is nullptr");
//    myTriangleColliders.emplace_back(aCollider);
//}
//
//void CCollisionManager::RegisterCollider(CCircleColliderComponent* aCollider) {
//    assert(aCollider != nullptr && "Collider is nullptr");
//    myCircleColliders.emplace_back(aCollider);
//}
//
//void CCollisionManager::RegisterCollider(CRectangleColliderComponent* aCollider) {
//    assert(aCollider != nullptr && "Collider is nullptr");
//    myRectangleColliders.emplace_back(aCollider);
//}

void CCollisionManager::RegisterCollider(CCollider* aCollider)
{
    assert(aCollider != nullptr && "Collider is nullptr");
    myColliders.emplace_back(aCollider);
}

void CCollisionManager::Update()
{
    ////Circle vs Circle collision test
    //for (UINT outer = 0; outer < myCircleColliders.size(); ++outer) {
    //    for (UINT inner = outer + 1; inner < myCircleColliders.size(); ++inner) {
    //        if (CIntersectionManager::CircleIntersection(*myCircleColliders[outer], *myCircleColliders[inner])) {
    //            myCircleColliders[outer]->GameObject().Collided(myCircleColliders[inner]->GameObject());
    //            myCircleColliders[inner]->GameObject().Collided(myCircleColliders[outer]->GameObject());
    //        }
    //    }
    //}

    ////Circle vs Rectangle collision test
    //for (UINT circle = 0; circle < myCircleColliders.size(); ++circle) {
    //    for (UINT rectangle = 0; rectangle < myRectangleColliders.size(); ++rectangle) {
    //        if (CIntersectionManager::CircleVsRectangleIntersection(*myCircleColliders[circle], *myRectangleColliders[rectangle])) {
    //            myCircleColliders[circle]->GameObject().Collided(myRectangleColliders[rectangle]->GameObject());
    //            myRectangleColliders[rectangle]->GameObject().Collided(myCircleColliders[circle]->GameObject());
    //        }
    //    }
    //}

    ////Circle vs Triangle collision test
    //for (UINT circle = 0; circle < myCircleColliders.size(); ++circle) {
    //    for (UINT triangle = 0; triangle < myTriangleColliders.size(); ++triangle) {
    //        if (CIntersectionManager::CircleVsTriangleIntersection(*myCircleColliders[circle], *myTriangleColliders[triangle])) {
    //            myCircleColliders[circle]->GameObject().Collided(myTriangleColliders[triangle]->GameObject());
    //            myTriangleColliders[triangle]->GameObject().Collided(myCircleColliders[circle]->GameObject());
    //        }
    //    }
    //}

    //Collider vs Collider collision test
    for (unsigned int outer = 0; outer < myColliders.size(); ++outer)
    {
        for (unsigned int inner = outer + 1; inner < myColliders.size(); ++inner)
        {
            if (myColliders[outer]->Collided(myColliders[inner]))
            {
                myColliders[outer]->GameObject().Collided(myColliders[inner]->GameObject());
                myColliders[inner]->GameObject().Collided(myColliders[outer]->GameObject());
            }
        }
    }
}