//#include "stdafx.h"
//#include "IntersectionManager.h"
//#include "CapsuleColliderComponent.h"
//#include "TriangleColliderComponent.h"
//#include "CircleColliderComponent.h"
//#include "RectangleColliderComponent.h"
//#include "SimpleMath.h"
//#include "Math.h"
//
//using namespace DirectX::SimpleMath;
//
//DirectX::SimpleMath::Vector3 CIntersectionManager::myCapsulePenetration = { 0.f, 0.f, 0.f };
//
//static bool colliderDebug = true;
//
//Vector3 CIntersectionManager::ClosestPointOnLineSegment(Vector3 aA, Vector3 aB, Vector3 aPoint)
//{
//	Vector3 AB = aB - aA;
//	float t = AB.Dot((aPoint - aA)) / AB.Dot(AB);
//	return aA + min(max(t, 0), 1) * AB; // saturate(t) can be written as: min((max(t, 0), 1)
//}
//
//bool CIntersectionManager::IntersectionTest(CCollider& aCollider, CCollider& aCollider2)
//{
//	/*if (aCollider.myCollisionLayer == aCollider2.myCollisionLayer) {
//		return false;
//	}*/
//
//	
//
//	//switch (aCollider.myColliderType)
//	//{
//	//case EColliderType::RECTANGLE:
//	//	return ActiveRectangle(*dynamic_cast<CRectangleColliderComponent*>(&aCollider), aCollider2);
//	//	break;
//	//case EColliderType::CIRCLE:
//	//	return ActiveCircle(*dynamic_cast<CCircleColliderComponent*>(&aCollider), aCollider2);
//	//	break;
//	//case EColliderType::TRIANGLE:
//	//	return ActiveTriangle(*dynamic_cast<CTriangleColliderComponent*>(&aCollider), aCollider2);
//	//	break;
//	//default:
//	//	break;
//	//}
//
//	//return Collision(&aCollider, &aCollider2);
//}
//
////bool CIntersectionManager::ActiveCircle(CCircleColliderComponent& aCircleCollider, CCollider& aCollider)
////{
////	switch (aCollider.myColliderType)
////	{
////	case EColliderType::RECTANGLE:
////		return ActiveCircleCollision(aCircleCollider, *dynamic_cast<CRectangleColliderComponent*>(&aCollider));
////		break;
////	case EColliderType::CIRCLE:
////		return ActiveCircleCollision(aCircleCollider, *dynamic_cast<CCircleColliderComponent*>(&aCollider));
////		break;
////	case EColliderType::TRIANGLE:
////		return ActiveCircleCollision(aCircleCollider, *dynamic_cast<CTriangleColliderComponent*>(&aCollider));
////		break;
////	default:
////		break;
////	}
////
////	return false;
////}
////
////bool CIntersectionManager::ActiveRectangle(CRectangleColliderComponent& aRectangleCollider, CCollider& aCollider)
////{
////	switch (aCollider.myColliderType)
////	{
////	case EColliderType::RECTANGLE:
////		return ActiveRectangleCollision(aRectangleCollider, *dynamic_cast<CRectangleColliderComponent*>(&aCollider));
////		break;
////	case EColliderType::CIRCLE:
////		return ActiveRectangleCollision(aRectangleCollider, *dynamic_cast<CCircleColliderComponent*>(&aCollider));
////		break;
////	case EColliderType::TRIANGLE:
////		return ActiveRectangleCollision(aRectangleCollider, *dynamic_cast<CTriangleColliderComponent*>(&aCollider));
////		break;
////	default:
////		break;
////	}
////
////	return false;
////}
////
////bool CIntersectionManager::ActiveTriangle(CTriangleColliderComponent& aTriangleCollider, CCollider& aCollider)
////{
////	switch (aCollider.myColliderType)
////	{
////	case EColliderType::RECTANGLE:
////		return ActiveTriangleCollision(aTriangleCollider, *dynamic_cast<CRectangleColliderComponent*>(&aCollider));
////		break;
////	case EColliderType::CIRCLE:
////		return ActiveTriangleCollision(aTriangleCollider, *dynamic_cast<CCircleColliderComponent*>(&aCollider));
////		break;
////	case EColliderType::TRIANGLE:
////		return ActiveTriangleCollision(aTriangleCollider, *dynamic_cast<CTriangleColliderComponent*>(&aCollider));
////		break;
////	default:
////		break;
////	}
////
////	return false;
////}
//
//bool CIntersectionManager::Collision(CCircleColliderComponent& aCircle, CCircleColliderComponent& aCircle2)
//{
//	//WÖRK
//	if (aCircle.myRadius + aCircle.myRadius < DirectX::SimpleMath::Vector3::Distance(aCircle.myPosition, aCircle2.myPosition)) {
//		if (colliderDebug)
//			std::cout << "no collision " << std::endl;
//		return false;
//	}
//	else {
//		if (colliderDebug)
//			std::cout << "collision " << std::endl;
//		return true;
//	}
//}
//
//bool CIntersectionManager::Collision(CCircleColliderComponent& aCircle, CRectangleColliderComponent& aRectangle)
//{
//	float squaredDistance = 0.0f;
//
//	if (aCircle.myPosition.x < aRectangle.myMin.x) {
//		squaredDistance += (aRectangle.myMin.x - aCircle.myPosition.x) * (aRectangle.myMin.x - aCircle.myPosition.x);
//	}
//	if (aCircle.myPosition.x > aRectangle.myMax.x) {
//		squaredDistance += (aCircle.myPosition.x - aRectangle.myMax.x) * (aCircle.myPosition.x - aRectangle.myMax.x);
//	}
//
//	if (aCircle.myPosition.z < aRectangle.myMin.z) {
//		squaredDistance += (aRectangle.myMin.z - aCircle.myPosition.z) * (aRectangle.myMin.z - aCircle.myPosition.z);
//	}
//	if (aCircle.myPosition.z > aRectangle.myMax.z) {
//		squaredDistance += (aCircle.myPosition.z - aRectangle.myMax.z) * (aCircle.myPosition.z - aRectangle.myMax.z);
//	}
//
//	if (squaredDistance <= aCircle.myRadius * aCircle.myRadius) {
//		return true; //Has collided
//	}
//	return false; //Has not collided
//}
//
//bool CIntersectionManager::Collision(CCircleColliderComponent& aCircle, CTriangleColliderComponent& aTriangle)
//{
//	//Test 1, Vertex within circle
//	float c1x = aCircle.myPosition.x - aTriangle.myPosition.x;
//	float c1z = aCircle.myPosition.z - aTriangle.myPosition.z;
//
//	float radiusSqr = aCircle.myRadius * aCircle.myRadius;
//	float c1sqr = c1x * c1x + c1z * c1z - radiusSqr;
//
//	if (c1sqr <= 0) {
//		if (colliderDebug)
//			std::cout << "collision at vertex 0 inside" << std::endl;
//		return true;
//	}
//
//	float c2x = aCircle.myPosition.x - aTriangle.myLeftVertex.x;
//	float c2z = aCircle.myPosition.z - aTriangle.myLeftVertex.z;
//	float c2sqr = c2x * c2x + c2z * c2z - radiusSqr;
//
//	if (c2sqr <= 0) {
//		if (colliderDebug)
//			std::cout << "collision at vertex 1 inside" << std::endl;
//		return true;
//	}
//
//	float c3x = aCircle.myPosition.x - aTriangle.myRightVertex.x;
//	float c3z = aCircle.myPosition.z - aTriangle.myRightVertex.z;
//
//	float c3sqr = c3x * c3x + c3z * c3z - radiusSqr;
//
//	if (c3sqr <= 0) {
//		if (colliderDebug)
//			std::cout << "collision at vertex 2 inside" << std::endl;
//		return true;
//	}
//
//	//Test 2, Circle centre within triangle
//	float e1x = aTriangle.myLeftVertex.x - aTriangle.myPosition.x;
//	float e1z = aTriangle.myLeftVertex.z - aTriangle.myPosition.z;
//
//	float e2x = aTriangle.myRightVertex.x - aTriangle.myLeftVertex.x;
//	float e2z = aTriangle.myRightVertex.z - aTriangle.myLeftVertex.z;
//
//	float e3x = aTriangle.myPosition.x - aTriangle.myRightVertex.x;
//	float e3z = aTriangle.myPosition.z - aTriangle.myRightVertex.z;
//
//	if (((e1z * c1x) - (e1x * c1z)) >= 0 || ((e2z * c2x) - (e2x * c2z)) >= 0 || ((e3z * c3x) - (e3x * c3z)) >= 0) {
//
//	}
//	else {
//		if (colliderDebug)
//			std::cout << "collision at centre in triangle" << std::endl;
//		return true;
//	}
//	//Test 3, Circle intersects edge
//	float k = c1x * e1x + c1z * e1z;
//
//	//First edge
//	if (k > 0) {
//		float len = e1x * e1x + e1z * e1z;
//
//		if (k < len) {
//			if (c1sqr * len <= k * k) {
//				if (colliderDebug)
//					std::cout << "collision at first edge" << std::endl;
//				return true;
//			}
//		}
//	}
//
//	//Second edge
//	k = c2x * e2x + c2z * e2z;
//
//	if (k > 0) {
//		float len = e2x * e2x + e2z * e2z;
//
//		if (k < len) {
//			if (c2sqr * len <= k * k) {
//				if (colliderDebug)
//					std::cout << "collision at second edge" << std::endl;
//				return true;
//			}
//		}
//	}
//
//	//Third edge
//	k = c3x * e3x + c3z * e3z;
//
//	if (k > 0) {
//		float len = e3x * e3x + e3z * e3z;
//
//		if (k < len) {
//			if (c3sqr * len <= k * k) {
//				if (colliderDebug)
//					std::cout << "collision at third edge" << std::endl;
//				return true;
//			}
//		}
//	}
//
//	//No haqvinsection #PHATCODE
//	if (colliderDebug) {
//		std::cout << "no collision " << std::endl;
//	}
//	return false;
//}
//
//bool CIntersectionManager::Collision(CRectangleColliderComponent& aRectangle, CRectangleColliderComponent& aRectangle2)
//{
//	return (aRectangle.myMin.x <= aRectangle2.myMax.x && aRectangle.myMax.x >= aRectangle2.myMin.x) &&
//	(aRectangle.myMin.z <= aRectangle2.myMax.z && aRectangle.myMax.z >= aRectangle2.myMin.z);
//}
//
//bool CIntersectionManager::Collision(CRectangleColliderComponent& aRectangle, CCircleColliderComponent& aCircle)
//{
//	float squaredDistance = 0.0f;
//
//	if (aCircle.myPosition.x < aRectangle.myMin.x) {
//		squaredDistance += (aRectangle.myMin.x - aCircle.myPosition.x) * (aRectangle.myMin.x - aCircle.myPosition.x);
//	}
//	if (aCircle.myPosition.x > aRectangle.myMax.x) {
//		squaredDistance += (aCircle.myPosition.x - aRectangle.myMax.x) * (aCircle.myPosition.x - aRectangle.myMax.x);
//	}
//
//	if (aCircle.myPosition.z < aRectangle.myMin.z) {
//		squaredDistance += (aRectangle.myMin.z - aCircle.myPosition.z) * (aRectangle.myMin.z - aCircle.myPosition.z);
//	}
//	if (aCircle.myPosition.z > aRectangle.myMax.z) {
//		squaredDistance += (aCircle.myPosition.z - aRectangle.myMax.z) * (aCircle.myPosition.z - aRectangle.myMax.z);
//	}
//
//	if (squaredDistance <= aCircle.myRadius * aCircle.myRadius) {
//		return true; //Has collided
//	}
//	return false; //Has not collided
//}
//
//bool CIntersectionManager::Collision(CRectangleColliderComponent& aRectangle, CTriangleColliderComponent& aTriangle)
//{
//	return false;
//}
//
//bool CIntersectionManager::Collision(CTriangleColliderComponent& aTriangle, CTriangleColliderComponent& aTriangle2)
//{
//	return false;
//}
//
//bool CIntersectionManager::Collision(CTriangleColliderComponent& aTriangle, CCircleColliderComponent& aCircle)
//{
//	//Test 1, Vertex within circle
//	float c1x = aCircle.myPosition.x - aTriangle.myPosition.x;
//	float c1z = aCircle.myPosition.z - aTriangle.myPosition.z;
//
//	float radiusSqr = aCircle.myRadius * aCircle.myRadius;
//	float c1sqr = c1x * c1x + c1z * c1z - radiusSqr;
//
//	if (c1sqr <= 0) {
//		if (colliderDebug)
//			std::cout << "collision at vertex 0 inside" << std::endl;
//		return true;
//	}
//
//	float c2x = aCircle.myPosition.x - aTriangle.myLeftVertex.x;
//	float c2z = aCircle.myPosition.z - aTriangle.myLeftVertex.z;
//	float c2sqr = c2x * c2x + c2z * c2z - radiusSqr;
//
//	if (c2sqr <= 0) {
//		if (colliderDebug)
//			std::cout << "collision at vertex 1 inside" << std::endl;
//		return true;
//	}
//
//	float c3x = aCircle.myPosition.x - aTriangle.myRightVertex.x;
//	float c3z = aCircle.myPosition.z - aTriangle.myRightVertex.z;
//
//	float c3sqr = c3x * c3x + c3z * c3z - radiusSqr;
//
//	if (c3sqr <= 0) {
//		if (colliderDebug)
//			std::cout << "collision at vertex 2 inside" << std::endl;
//		return true;
//	}
//
//	//Test 2, Circle centre within triangle
//	float e1x = aTriangle.myLeftVertex.x - aTriangle.myPosition.x;
//	float e1z = aTriangle.myLeftVertex.z - aTriangle.myPosition.z;
//
//	float e2x = aTriangle.myRightVertex.x - aTriangle.myLeftVertex.x;
//	float e2z = aTriangle.myRightVertex.z - aTriangle.myLeftVertex.z;
//
//	float e3x = aTriangle.myPosition.x - aTriangle.myRightVertex.x;
//	float e3z = aTriangle.myPosition.z - aTriangle.myRightVertex.z;
//
//	if (((e1z * c1x) - (e1x * c1z)) >= 0 || ((e2z * c2x) - (e2x * c2z)) >= 0 || ((e3z * c3x) - (e3x * c3z)) >= 0) {
//
//	}
//	else {
//		if (colliderDebug)
//			std::cout << "collision at centre in triangle" << std::endl;
//		return true;
//	}
//	//Test 3, Circle intersects edge
//	float k = c1x * e1x + c1z * e1z;
//
//	//First edge
//	if (k > 0) {
//		float len = e1x * e1x + e1z * e1z;
//
//		if (k < len) {
//			if (c1sqr * len <= k * k) {
//				if (colliderDebug)
//					std::cout << "collision at first edge" << std::endl;
//				return true;
//			}
//		}
//	}
//
//	//Second edge
//	k = c2x * e2x + c2z * e2z;
//
//	if (k > 0) {
//		float len = e2x * e2x + e2z * e2z;
//
//		if (k < len) {
//			if (c2sqr * len <= k * k) {
//				if (colliderDebug)
//					std::cout << "collision at second edge" << std::endl;
//				return true;
//			}
//		}
//	}
//
//	//Third edge
//	k = c3x * e3x + c3z * e3z;
//
//	if (k > 0) {
//		float len = e3x * e3x + e3z * e3z;
//
//		if (k < len) {
//			if (c3sqr * len <= k * k) {
//				if (colliderDebug)
//					std::cout << "collision at third edge" << std::endl;
//				return true;
//			}
//		}
//	}
//
//	//No haqvinsection #PHATCODE
//	if (colliderDebug) {
//		std::cout << "no collision " << std::endl;
//	}
//	return false;
//}
//
//bool CIntersectionManager::Collision(CTriangleColliderComponent& aTriangle, CRectangleColliderComponent& aRectangle)
//{
//	return false;
//}
