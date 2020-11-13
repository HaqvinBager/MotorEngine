//#pragma once
//
//namespace DirectX {
//	namespace SimpleMath {
//		struct Vector3;
//	}
//}
//
//class CCapsuleColliderComponent;
//class CTriangleColliderComponent;
//class CCircleColliderComponent;
//class CRectangleColliderComponent;
//
//class CIntersectionManager
//{
//public:
//	//static bool RectangleIntersection(CRectangleColliderComponent& aRectangle, CRectangleColliderComponent& aRectangle2);
//	//static bool IntersectionTest(CCollider& aCollider, CCollider& aCollider2);
//
//private:
//	//static bool ActiveCircle(CCircleColliderComponent& aCircleCollider, CCollider& aCollider);
//	//static bool ActiveRectangle(CRectangleColliderComponent& aRectangleCollider, CCollider& aCollider);
//	//static bool ActiveTriangle(CTriangleColliderComponent& aTriangleCollider, CCollider& aCollider);
//
////	//Circle first
////	static bool Collision(CCircleColliderComponent& aCircle, CCircleColliderComponent& aCircle2);
////	static bool Collision(CCircleColliderComponent& aCircle, CRectangleColliderComponent& aRectangle);
////	static bool Collision(CCircleColliderComponent& aCircle, CTriangleColliderComponent& aTriangle);
////
////	//Rectangle first
////	static bool Collision(CRectangleColliderComponent& aRectangle, CRectangleColliderComponent& aRectangle2);
////	static bool Collision(CRectangleColliderComponent& aRectangle, CCircleColliderComponent& aCircle);
////	static bool Collision(CRectangleColliderComponent& aRectangle, CTriangleColliderComponent& aTriangle);
////	
////	//Triangle first
////	static bool Collision(CTriangleColliderComponent&aTriangle, CTriangleColliderComponent& aTriangle2);
////	static bool Collision(CTriangleColliderComponent&aTriangle, CCircleColliderComponent& aCircle);
////	static bool Collision(CTriangleColliderComponent& aTriangle, CRectangleColliderComponent& aRectangle);
////
////	//static bool IntersectionTest(CCapsuleColliderComponent& aCapsule, CCapsuleColliderComponent& aCapsule2);
////	//static bool IntersectionTest(CCircleColliderComponent& aCircle, DirectX::SimpleMath::Ray aRay);
////	//static bool IntersectionTest
////	static DirectX::SimpleMath::Vector3 GetCapsulePenetration() { return myCapsulePenetration; }
////
////private:
////	static DirectX::SimpleMath::Vector3 ClosestPointOnLineSegment(DirectX::SimpleMath::Vector3 aA, DirectX::SimpleMath::Vector3 aB, DirectX::SimpleMath::Vector3 aPoint);
////	static DirectX::SimpleMath::Vector3 myCapsulePenetration;
//};
//
