#pragma once

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CCapsuleColliderComponent;

class CIntersectionManager
{
public:
	static bool CapsuleIntersection(const CCapsuleColliderComponent& aCapsule, const CCapsuleColliderComponent& aCapsule2);
	static DirectX::SimpleMath::Vector3 ClosestPointOnLineSegment(DirectX::SimpleMath::Vector3 aA, DirectX::SimpleMath::Vector3 aB, DirectX::SimpleMath::Vector3 aPoint);
private:
};

