#include "stdafx.h"
#include "RectangleColliderComponent.h"
#include "TriangleColliderComponent.h"
#include "CircleColliderComponent.h"
#include "CollisionManager.h"
#include "TransformComponent.h"

CRectangleColliderComponent::CRectangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight, ECollisionLayer aCollisionLayer) :
	CCollider(aParent),
	myWidth(aWidth),
	myHeight(aHeight)
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
	SetCollisionLayer(aCollisionLayer);
}

CRectangleColliderComponent::~CRectangleColliderComponent() {
}

void CRectangleColliderComponent::Awake() {

	DirectX::SimpleMath::Vector3 vector = GameObject().GetComponent<CTransformComponent>()->Position() + GameObject().GetComponent<CTransformComponent>()->Transform().Forward() * (myHeight / 2.0f);

	SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());
	
	//TODO Fix after Merge Conflicts Resolved
	myMin = vector - GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
	//myMin.z = vector + GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
	
	vector = GameObject().GetComponent<CTransformComponent>()->Position() - GameObject().GetComponent<CTransformComponent>()->Transform().Forward() * (myHeight / 2.0f);
	
	//TODO Fix after Merge Conflicts Resolved
	//myVertices.emplace_back(vector - GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
	myMax = vector + GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
}

void CRectangleColliderComponent::Start() {
}

#include "Debug.h"
void CRectangleColliderComponent::Update() {
	
	
	DirectX::SimpleMath::Vector3 vector = GameObject().GetComponent<CTransformComponent>()->Position() + GameObject().GetComponent<CTransformComponent>()->Transform().Forward() * (myHeight / 2.0f);

	SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());

	//TODO Fix after Merge Conflicts Resolved
	myMin = vector - GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
	//myMin.z = vector + GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));

	vector = GameObject().GetComponent<CTransformComponent>()->Position() - GameObject().GetComponent<CTransformComponent>()->Transform().Forward() * (myHeight / 2.0f);

	//TODO Fix after Merge Conflicts Resolved
	//myVertices.emplace_back(vector - GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
	myMax = vector + GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);

	if (GetAsyncKeyState('C')) {
		CDebug::GetInstance()->DrawLine(myMin, myMax);
	}
}

bool CRectangleColliderComponent::Collided(CCircleColliderComponent* aCollidedGameObject)
{
	float squaredDistance = 0.0f;

	if (aCollidedGameObject->GetPosition().x < this->GetMin().x) {
		squaredDistance += (this->GetMin().x - aCollidedGameObject->GetPosition().x) * (this->GetMin().x - aCollidedGameObject->GetPosition().x);
	}
	if (aCollidedGameObject->GetPosition().x > this->GetMax().x) {
		squaredDistance += (aCollidedGameObject->GetPosition().x - this->GetMax().x) * (aCollidedGameObject->GetPosition().x - this->GetMax().x);
	}

	if (aCollidedGameObject->GetPosition().z < this->GetMin().z) {
		squaredDistance += (this->GetMin().z - aCollidedGameObject->GetPosition().z) * (this->GetMin().z - aCollidedGameObject->GetPosition().z);
	}
	if (aCollidedGameObject->GetPosition().z > this->GetMax().z) {
		squaredDistance += (aCollidedGameObject->GetPosition().z - this->GetMax().z) * (aCollidedGameObject->GetPosition().z - this->GetMax().z);
	}

	if (squaredDistance <= aCollidedGameObject->GetRadius() * aCollidedGameObject->GetRadius()) {
		return true; //Has collided
	}
	return false; //Has not collided
}

bool CRectangleColliderComponent::Collided(CTriangleColliderComponent* /*aCollidedGameObject*/)
{
	//NO INTERSECTION TEST IMPLEMENTED AS OF YET
	return false;
}

bool CRectangleColliderComponent::Collided(CRectangleColliderComponent* aCollidedGameObject)
{
	return (this->myMin.x <= aCollidedGameObject->GetMax().x && this->myMax.x >= aCollidedGameObject->GetMin().x) &&
		(this->myMin.z <= aCollidedGameObject->GetMax().z && this->myMax.z >= aCollidedGameObject->GetMin().z);
}

bool CRectangleColliderComponent::Collided(CCollider* aCollidedGameObject)
{
	return aCollidedGameObject->Collided(this);
}

void CRectangleColliderComponent::OnEnable()
{
}

void CRectangleColliderComponent::OnDisable()
{
}
