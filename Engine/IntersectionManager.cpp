#include "stdafx.h"
#include "IntersectionManager.h"
#include "CapsuleColliderComponent.h"
#include "TriangleColliderComponent.h"
#include "CircleColliderComponent.h"
#include "RectangleColliderComponent.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

bool CIntersectionManager::RectangleIntersection(CRectangleColliderComponent& aRectangle, CRectangleColliderComponent& aRectangle2) {
	if (aRectangle.myMaxPoint.x < aRectangle2.myMinPoint.x) return false;
	if (aRectangle.myMinPoint.x > aRectangle2.myMaxPoint.x) return false;
	if (aRectangle.myMaxPoint.y < aRectangle2.myMinPoint.y) return false;
	if (aRectangle.myMinPoint.y > aRectangle2.myMaxPoint.y) return false;
	return true;
}

bool CIntersectionManager::CircleIntersection(CCircleColliderComponent& aCircle, CCircleColliderComponent& aCircle2) {
	if (aCircle.myRadius + aCircle.myRadius < DirectX::SimpleMath::Vector3::Distance(aCircle.myPosition, aCircle2.myPosition))
		return false;

	return true;
}

bool CIntersectionManager::RectangleVsCircleIntersection(CRectangleColliderComponent& aRectangle, CCircleColliderComponent& aCircle) {
	Vector2 circleDistance = { abs(aCircle.myPosition.x - aRectangle.myPosition.x), abs(aCircle.myPosition.y - aRectangle.myPosition.y) };

	if (circleDistance.x > (aRectangle.myColliderSize.x / 2.0f + aCircle.myRadius)) return false;
	if (circleDistance.y > (aRectangle.myColliderSize.y / 2.0f + aCircle.myRadius)) return false;

	if (circleDistance.x <= (aRectangle.myColliderSize.x / 2.0f)) return true;
	if (circleDistance.y <= (aRectangle.myColliderSize.y / 2.0f)) return true;

	float cornerDistance_sq = pow((circleDistance.x - aRectangle.myColliderSize.x / 2.0f), 2) + pow((circleDistance.y - aRectangle.myColliderSize.y / 2.0f), 2);

	return (cornerDistance_sq <= (pow(aCircle.myRadius, 2)));
}

bool CIntersectionManager::CircleVsTriangleIntersection(CCircleColliderComponent& aCircle, CTriangleColliderComponent& aTriangle) {
	//Test 1, Vertex within circle
	float c1x = aCircle.myPosition.x - aTriangle.myVertices[0].x;
	float c1z = aCircle.myPosition.z - aTriangle.myVertices[0].z;

	float radiusSqr = aCircle.myRadius * aCircle.myRadius;
	float c1sqr = c1x * c1x + c1z * c1z - radiusSqr;

	if (c1sqr <= 0) return true;

	float c2x = aCircle.myPosition.x - aTriangle.myVertices[1].x;
	float c2z = aCircle.myPosition.z - aTriangle.myVertices[1].z;
	float c2sqr = c2x * c2x + c2z * c2z - radiusSqr;

	if (c2sqr <= 0) return true;

	float c3x = aCircle.myPosition.x - aTriangle.myVertices[2].x;
	float c3z = aCircle.myPosition.z - aTriangle.myVertices[2].z;

	float c3sqr = c3x * c3x + c3z * c3z - radiusSqr;

	if (c3sqr <= 0) return true;

	//Test 2, Circle centre within triangle
	float e1x = aTriangle.myVertices[1].x - aTriangle.myVertices[0].x;
	float e1z = aTriangle.myVertices[1].z - aTriangle.myVertices[0].z;

	float e2x = aTriangle.myVertices[2].x - aTriangle.myVertices[1].x;
	float e2z = aTriangle.myVertices[2].z - aTriangle.myVertices[1].z;

	float e3x = aTriangle.myVertices[0].x - aTriangle.myVertices[2].x;
	float e3z = aTriangle.myVertices[0].z - aTriangle.myVertices[2].z;

	if (signed((e1z * c1x - e1x * c1z) || (e2z * c2x - e2x * c2z) || (e3z * c3x - e3x * c3z)) >= 0) return true;

	//Test 3, Circle intersects edge
	float k = c1x * e1x + c1z * e1z;

	//First edge
	if (k > 0) {
		float len = e1x * e1x + e1z * e1z;

		if (k < len) {
			if (c1sqr * len <= k * k) return true;
		}
	}

	//Second edge
	k = c2x * e2x + c2z * e2z;

	if (k > 0) {
		float len = e2x * e2x + e2z * e2z;

		if (k < len) {
			if (c2sqr * len <= k * k) return true;
		}
	}

	//Third edge
	k = c3x * e3x + c3z * e3z;

	if (k > 0) {
		float len = e3x * e3x + e3z * e3z;

		if (k < len) {
			if (c3sqr * len <= k * k) return true;
		}
	}

	//No haqvinsection #PHATCODE
	return false;
}

bool CIntersectionManager::CapsuleIntersection(CCapsuleColliderComponent& aCapsuleA, CCapsuleColliderComponent& aCapsuleB)
{
	//Capsule A
	Vector3 normalA = (aCapsuleA.myTip - aCapsuleA.myBase);
	normalA.Normalize();
	Vector3 lineEndOffsetA = normalA * aCapsuleA.myRadius;
	Vector3 endOfBotCylinderA = aCapsuleA.myBase + lineEndOffsetA;
	Vector3 endOfTopCylinderA = aCapsuleA.myTip - lineEndOffsetA;

	//Capsule B
	Vector3 normalB = (aCapsuleB.myTip - aCapsuleB.myBase);
	normalB.Normalize();
	Vector3 lineEndOffsetB = normalB * aCapsuleB.myRadius;
	Vector3 endOfBotCylinderB = aCapsuleB.myBase + lineEndOffsetB;
	Vector3 endOfTopCylinderB = aCapsuleB.myTip - lineEndOffsetB;

	Vector3 v0 = endOfBotCylinderB - endOfBotCylinderA;
	Vector3 v1 = endOfBotCylinderB - endOfTopCylinderA;
	Vector3 v2 = endOfTopCylinderB - endOfBotCylinderA;
	Vector3 v3 = endOfTopCylinderB - endOfTopCylinderA;

	float distance0 = v0.Dot(v0);
	float distance1 = v1.Dot(v1);
	float distance2 = v2.Dot(v2);
	float distance3 = v3.Dot(v3);

	Vector3 bestA;
	if (distance2 < distance0 || distance2 < distance1 || distance3 < distance0 || distance3 < distance1) {
		bestA = endOfTopCylinderA;
	}
	else {
		bestA = endOfBotCylinderA;
	}

	Vector3 bestB = ClosestPointOnLineSegment(endOfBotCylinderB, endOfTopCylinderB, bestA);
	bestA = ClosestPointOnLineSegment(endOfBotCylinderA, endOfTopCylinderA, bestB);

	Vector3 aPenetrationNormal = bestA - bestB;
	float aLength = aPenetrationNormal.Length();
	aPenetrationNormal.Normalize();
	float aPenetrationDepth = aCapsuleA.myRadius + aCapsuleB.myRadius - aLength;
	
	if (aPenetrationDepth > 0) {
		return true;
	}
	else {
		return false;
	}

}

Vector3 CIntersectionManager::ClosestPointOnLineSegment(Vector3 aA, Vector3 aB, Vector3 aPoint)
{
	Vector3 AB = aB - aA;
	float t = AB.Dot((aPoint - aA)) / AB.Dot(AB);
	return aA + min(max(t, 0), 1) * AB; // saturate(t) can be written as: min((max(t, 0), 1)
}
