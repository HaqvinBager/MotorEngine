#include "stdafx.h"
#include "Debug.h"
#include "TriangleColliderComponent.h"
#include "CircleColliderComponent.h"
#include "RectangleColliderComponent.h"
#include "CollisionManager.h"
#include "TransformComponent.h"

CTriangleColliderComponent::CTriangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags) :
	CCollider(aParent, aCollisionLayer, someCollisionFlags),
	myWidth(aWidth),
	myHeight(aHeight)
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
	SetCollisionLayer(aCollisionLayer);
}

CTriangleColliderComponent::~CTriangleColliderComponent() {
}


void CTriangleColliderComponent::Awake() {
	DirectX::SimpleMath::Vector3 vector = GameObject().GetComponent<CTransformComponent>()->Position() + GameObject().GetComponent<CTransformComponent>()->Transform().Forward() * myHeight;

	SetPosition(GameObject().GetComponent<CTransformComponent>()->Position()); //Verex 0
	myLeftVertex = (vector - GameObject().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f)); //Verex 1
	myRightVertex = (vector + GameObject().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f)); //Verex 2
}

void CTriangleColliderComponent::Start() {
}

void CTriangleColliderComponent::Update() {
	DirectX::SimpleMath::Vector3 vector = GameObject().GetComponent<CTransformComponent>()->Position() + GameObject().GetComponent<CTransformComponent>()->Transform().Forward() * myHeight;
	SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());
	myLeftVertex = vector - GameObject().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f);
	myRightVertex = vector + GameObject().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f);

	if (GetAsyncKeyState('C')) {

		CDebug::GetInstance()->DrawLine(GetPosition(), myLeftVertex);
		CDebug::GetInstance()->DrawLine(GetPosition(), myRightVertex);
		CDebug::GetInstance()->DrawLine(myLeftVertex, myRightVertex);
	}
}

bool CTriangleColliderComponent::Collided(CCircleColliderComponent* aCollidedGameObject)
{
	//Test 1, Vertex within circle
	float c1x = aCollidedGameObject->GetPosition().x - this->GetPosition().x;
	float c1z = aCollidedGameObject->GetPosition().z - this->GetPosition().z;

	float radiusSqr = aCollidedGameObject->GetRadius() * aCollidedGameObject->GetRadius();
	float c1sqr = c1x * c1x + c1z * c1z - radiusSqr;

	if (c1sqr <= 0) {
		return true;
	}

	float c2x = aCollidedGameObject->GetPosition().x - this->myLeftVertex.x;
	float c2z = aCollidedGameObject->GetPosition().z - this->myLeftVertex.z;
	float c2sqr = c2x * c2x + c2z * c2z - radiusSqr;

	if (c2sqr <= 0) {
		return true;
	}

	float c3x = aCollidedGameObject->GetPosition().x - this->myRightVertex.x;
	float c3z = aCollidedGameObject->GetPosition().z - this->myRightVertex.z;

	float c3sqr = c3x * c3x + c3z * c3z - radiusSqr;

	if (c3sqr <= 0) {
		return true;
	}

	//Test 2, Circle centre within triangle
	float e1x = this->myLeftVertex.x - this->GetPosition().x;
	float e1z = this->myLeftVertex.z - this->GetPosition().z;
	float e2x = this->myRightVertex.x - this->myLeftVertex.x;
	float e2z = this->myRightVertex.z - this->myLeftVertex.z;
	float e3x = this->GetPosition().x - this->myRightVertex.x;
	float e3z = this->GetPosition().z - this->myRightVertex.z;

	if (((e1z * c1x) - (e1x * c1z)) >= 0 || ((e2z * c2x) - (e2x * c2z)) >= 0 || ((e3z * c3x) - (e3x * c3z)) >= 0) {

	}
	else {
		return true;
	}
	//Test 3, Circle intersects edge
	float k = c1x * e1x + c1z * e1z;

	//First edge
	if (k > 0) {
		float len = e1x * e1x + e1z * e1z;

		if (k < len) {
			if (c1sqr * len <= k * k) {
				return true;
			}
		}
	}

	//Second edge
	k = c2x * e2x + c2z * e2z;

	if (k > 0) {
		float len = e2x * e2x + e2z * e2z;

		if (k < len) {
			if (c2sqr * len <= k * k) {
				return true;
			}
		}
	}

	//Third edge
	k = c3x * e3x + c3z * e3z;

	if (k > 0) {
		float len = e3x * e3x + e3z * e3z;

		if (k < len) {
			if (c3sqr * len <= k * k) {
				return true;
			}
		}
	}

	//No haqvinsection #PHATCODE
	return false;
}

bool CTriangleColliderComponent::Collided(CTriangleColliderComponent* /*aCollidedGameObject*/)
{
	//NO INTERSECTION TEST IMPLEMENTED AS OF YET
	return false;
}

bool CTriangleColliderComponent::Collided(CRectangleColliderComponent* /*aCollidedGameObject*/)
{
	//NO INTERSECTION TEST IMPLEMENTED AS OF YET
	return false;
}

bool CTriangleColliderComponent::Collided(CCollider* aCollidedGameObject)
{
	return aCollidedGameObject->Collided(this);
}

void CTriangleColliderComponent::OnEnable()
{
}

void CTriangleColliderComponent::OnDisable()
{
}
