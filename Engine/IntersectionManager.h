#pragma once

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CCapsuleColliderComponent;
class CTriangleColliderComponent;
class CCircleColliderComponent;
class CRectangleColliderComponent;

class CIntersectionManager
{
public:
	//static bool RectangleIntersection(CRectangleColliderComponent& aRectangle, CRectangleColliderComponent& aRectangle2);
	static bool CircleIntersection(CCircleColliderComponent& aCircle, CCircleColliderComponent& aCircle2);
	static bool CircleVsRectangleIntersection(CCircleColliderComponent& aCircle, CRectangleColliderComponent& aRectangle);
	static bool CircleVsTriangleIntersection(CCircleColliderComponent& aCircle, CTriangleColliderComponent& aTriangle);
	static bool CapsuleIntersection(CCapsuleColliderComponent& aCapsule, CCapsuleColliderComponent& aCapsule2);
	static bool CircleVsRay(CCircleColliderComponent& aCircle, DirectX::SimpleMath::Ray aRay);

private:
	static DirectX::SimpleMath::Vector3 ClosestPointOnLineSegment(DirectX::SimpleMath::Vector3 aA, DirectX::SimpleMath::Vector3 aB, DirectX::SimpleMath::Vector3 aPoint);
};

