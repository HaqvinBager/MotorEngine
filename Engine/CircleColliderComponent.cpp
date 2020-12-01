#include "stdafx.h"
#include "CircleColliderComponent.h"
#include "TriangleColliderComponent.h"
#include "RectangleColliderComponent.h"
#include "CollisionManager.h"
#include "TransformComponent.h"
#include "Debug.h"

#ifdef _DEBUG
#include "LineInstance.h"
#include "LineFactory.h"
#include "Engine.h"
#include "Scene.h"
#endif

CCircleColliderComponent::CCircleColliderComponent(CGameObject& aParent) :
	CCollider(aParent, ECollisionLayer::NONE, static_cast<int>(ECollisionLayer::NONE)),
	myRadius(0.5f)
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
#ifdef _DEBUG
	myVisualizer = nullptr;
#endif
}

CCircleColliderComponent::CCircleColliderComponent(CGameObject& aParent, float aRadius) :
	CCollider(aParent, ECollisionLayer::NONE, static_cast<int>(ECollisionLayer::NONE)),
	myRadius(aRadius)
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
#ifdef _DEBUG
	myVisualizer = nullptr;
#endif
}

CCircleColliderComponent::CCircleColliderComponent(CGameObject& aParent, float aRadius, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags) :
	CCollider(aParent, aCollisionLayer, someCollisionFlags),
	myRadius(aRadius)
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
#ifdef _DEBUG
	myVisualizer = nullptr;
#endif
}

CCircleColliderComponent::~CCircleColliderComponent() {
#ifdef _DEBUG
	delete myVisualizer;
	myVisualizer = nullptr;
#endif // _DEBUG
}

void CCircleColliderComponent::Awake() {
	SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());
#ifdef _DEBUG
	myVisualizer = new CLineInstance();
	myVisualizer->Init(CLineFactory::GetInstance()->CreateCircleXZ(myRadius));
	myVisualizer->SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());
	CEngine::GetInstance()->GetActiveScene().AddInstance(myVisualizer);
#endif // _DEBUG
}

void CCircleColliderComponent::Start() {
}

void CCircleColliderComponent::Update() {
	SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());

#ifdef _DEBUG
	myVisualizer->SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());
	myVisualizer->SetRotation(GameObject().GetComponent<CTransformComponent>()->Rotation());
#endif // _DEBUG

	if (GetAsyncKeyState('C')) {
		CDebug::GetInstance()->DrawLine({ GetPosition().x - (myRadius), GetPosition().y, GetPosition().z }, { GetPosition().x + (myRadius), GetPosition().y, GetPosition().z });
		CDebug::GetInstance()->DrawLine({ GetPosition().x, GetPosition().y, GetPosition().z - (myRadius) }, { GetPosition().x, GetPosition().y, GetPosition().z + (myRadius) });
	}
}

bool CCircleColliderComponent::Collided(CCircleColliderComponent* aCollidedGameObject)
{
	if (this->myRadius + aCollidedGameObject->GetRadius() < 
		DirectX::SimpleMath::Vector3::Distance(this->GetPosition(), aCollidedGameObject->GetPosition()))
		return false;
	else
		return true;
}

bool CCircleColliderComponent::Collided(CRectangleColliderComponent* aCollidedGameObject)
{
	float squaredDistance = 0.0f;

	if (this->GetPosition().x < aCollidedGameObject->GetMin().x) {
		squaredDistance += (aCollidedGameObject->GetMin().x - this->GetPosition().x) * (aCollidedGameObject->GetMin().x - this->GetPosition().x);
	}
	if (this->GetPosition().x > aCollidedGameObject->GetMax().x) {
		squaredDistance += (this->GetPosition().x - aCollidedGameObject->GetMax().x) * (this->GetPosition().x - aCollidedGameObject->GetMax().x);
	}

	if (this->GetPosition().z < aCollidedGameObject->GetMin().z) {
		squaredDistance += (aCollidedGameObject->GetMin().z - this->GetPosition().z) * (aCollidedGameObject->GetMin().z - this->GetPosition().z);
	}
	if (this->GetPosition().z > aCollidedGameObject->GetMax().z) {
		squaredDistance += (this->GetPosition().z - aCollidedGameObject->GetMax().z) * (this->GetPosition().z - aCollidedGameObject->GetMax().z);
	}

	if (squaredDistance <= this->myRadius * this->myRadius) {
		return true; //Has collided
	}
	return false; //Has not collided
}

bool CCircleColliderComponent::Collided(CTriangleColliderComponent* aCollidedGameObject)
{
	aCollidedGameObject;
	//Test 1, Vertex within circle
	float c1x = this->GetPosition().x - aCollidedGameObject->GetPosition().x;
	float c1z = this->GetPosition().z - aCollidedGameObject->GetPosition().z;

	float radiusSqr = this->GetRadius() * this->GetRadius();
	float c1sqr = c1x * c1x + c1z * c1z - radiusSqr;

	if (c1sqr <= 0) {
		return true;
	}

	float c2x = this->GetPosition().x - aCollidedGameObject->GetLeftVertex().x;
	float c2z = this->GetPosition().z - aCollidedGameObject->GetLeftVertex().z;
	float c2sqr = c2x * c2x + c2z * c2z - radiusSqr;

	if (c2sqr <= 0) {
		return true;
	}

	float c3x = this->GetPosition().x - aCollidedGameObject->GetRightVertex().x;
	float c3z = this->GetPosition().z - aCollidedGameObject->GetRightVertex().z;

	float c3sqr = c3x * c3x + c3z * c3z - radiusSqr;

	if (c3sqr <= 0) {
		return true;
	}

	//Test 2, Circle centre within triangle
	float e1x = aCollidedGameObject->GetLeftVertex().x - aCollidedGameObject->GetPosition().x;
	float e1z = aCollidedGameObject->GetLeftVertex().z - aCollidedGameObject->GetPosition().z;
	float e2x = aCollidedGameObject->GetRightVertex().x - aCollidedGameObject->GetLeftVertex().x;
	float e2z = aCollidedGameObject->GetRightVertex().z - aCollidedGameObject->GetLeftVertex().z;
	float e3x = aCollidedGameObject->GetPosition().x - aCollidedGameObject->GetRightVertex().x;
	float e3z = aCollidedGameObject->GetPosition().z - aCollidedGameObject->GetRightVertex().z;

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

bool CCircleColliderComponent::Collided(CCollider* aCollidedGameObject) {
	return aCollidedGameObject->Collided(this);
}

void CCircleColliderComponent::OnEnable()
{
#ifdef _DEBUG
	if (myVisualizer)
	{
		myVisualizer->SetIsActive(true);
	}
#endif // _DEBUG
}
void CCircleColliderComponent::OnDisable()
{
#ifdef _DEBUG
	if (myVisualizer)
	{
		myVisualizer->SetPosition(CLineInstance::ourInactivePos);
		myVisualizer->SetIsActive(false);
	}
#endif // _DEBUG
}

float const CCircleColliderComponent::GetRadius() const
{
	return myRadius;
}

void CCircleColliderComponent::SetRadius(float aRadius)
{
	myRadius = aRadius;
}
