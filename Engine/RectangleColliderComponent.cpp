#include "stdafx.h"
#include "RectangleColliderComponent.h"
#include "TriangleColliderComponent.h"
#include "CircleColliderComponent.h"
#include "TransformComponent.h"
#include "Debug.h"

#include "CollisionManager.h"
#include "MainSingleton.h"

#ifdef _DEBUG
#include "LineInstance.h"
#include "LineFactory.h"
#include "Engine.h"
#include "Scene.h"
#endif

CRectangleColliderComponent::CRectangleColliderComponent(CGameObject& aParent) :
	CCollider(aParent, ECollisionLayer::NONE, static_cast<int>(ECollisionLayer::NONE)),
	myWidth(0.5f),
	myHeight(0.5f)
{
	
	CMainSingleton::CollisionManager().RegisterCollider(this);
#ifdef _DEBUG
	myVisualizer = nullptr;
#endif
}

CRectangleColliderComponent::CRectangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight) :
	CCollider(aParent, ECollisionLayer::NONE, static_cast<int>(ECollisionLayer::NONE)),
	myWidth(aWidth),
	myHeight(aHeight)
{
	CMainSingleton::CollisionManager().RegisterCollider(this);
#ifdef _DEBUG
	myVisualizer = nullptr;
#endif
}

CRectangleColliderComponent::CRectangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags) :
	CCollider(aParent, aCollisionLayer, someCollisionFlags),
	myWidth(aWidth),
	myHeight(aHeight)
{
	CMainSingleton::CollisionManager().RegisterCollider(this);
#ifdef _DEBUG
	myVisualizer = nullptr;
#endif
}

CRectangleColliderComponent::~CRectangleColliderComponent() {
#ifdef _DEBUG
	delete myVisualizer;
	myVisualizer = nullptr;
#endif // _DEBUG
}

void CRectangleColliderComponent::Awake() {

	DirectX::SimpleMath::Vector3 vector = GameObject().GetComponent<CTransformComponent>()->Position() + GameObject().GetComponent<CTransformComponent>()->Transform().Forward() * (myHeight / 2.0f) * 100.0f;

	SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());
	
	//TODO Fix after Merge Conflicts Resolved
	myMin = vector - GameObject().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f) * 100.0f;
	//myMin.z = vector + GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
	
	vector = GameObject().GetComponent<CTransformComponent>()->Position() - GameObject().GetComponent<CTransformComponent>()->Transform().Forward() * (myHeight / 2.0f) * 100.0f;
	
	//TODO Fix after Merge Conflicts Resolved
	//myVertices.emplace_back(vector - GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
	myMax = vector + GameObject().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f) * 100.0f;

#ifdef _DEBUG
	myVisualizer = new CLineInstance();
	myVisualizer->Init(CLineFactory::GetInstance()->CreateSquareXZ(myWidth));
	myVisualizer->SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());
	myVisualizer->SetRotation(GameObject().GetComponent<CTransformComponent>()->Rotation());
	CEngine::GetInstance()->GetActiveScene().AddInstance(myVisualizer);
#endif // _DEBUG
}

void CRectangleColliderComponent::Start() {
}

void CRectangleColliderComponent::Update() 
{
	// Position is center
	// myHeight is Length
	// myWidth is Width

	DirectX::SimpleMath::Vector3 vector = GameObject().GetComponent<CTransformComponent>()->Position() + GameObject().GetComponent<CTransformComponent>()->Transform().Forward() * (myHeight / 2.0f) * 100.0f;

	SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());

	//TODO Fix after Merge Conflicts Resolved
	myMin = vector - GameObject().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f) * 100.0f;
	//myMin.z = vector + GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));

	vector = GameObject().GetComponent<CTransformComponent>()->Position() - GameObject().GetComponent<CTransformComponent>()->Transform().Forward() * (myHeight / 2.0f) * 100.0f;

	//TODO Fix after Merge Conflicts Resolved
	//myVertices.emplace_back(vector - GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
	myMax = vector + GameObject().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f) * 100.0f;

#ifdef _DEBUG
	myVisualizer->SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());
	myVisualizer->SetRotation(GameObject().GetComponent<CTransformComponent>()->Rotation());
#endif // _DEBUG

	if (GetAsyncKeyState('C')) {
		CDebug::GetInstance()->DrawLine(myMin, { myMax.x, 0.0f, myMin.z }, 0.00000000001f);
		CDebug::GetInstance()->DrawLine(myMin, { myMin.x, 0.0f, myMax.z }, 0.00000000001f);
		CDebug::GetInstance()->DrawLine({ myMax.x, 0.0f, myMin.z }, myMax, 0.00000000001f);
		CDebug::GetInstance()->DrawLine({ myMin.x, 0.0f, myMax.z }, myMax, 0.00000000001f);
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
#ifdef _DEBUG
	auto thisMinX = this->myMin.x;
	auto thisMaxX = this->myMax.x;
	auto thisMinZ = this->myMin.z;
	auto thisMaxZ = this->myMax.z;

	auto theirMinX = aCollidedGameObject->GetMin().x;
	auto theirMaxX = aCollidedGameObject->GetMax().x;
	auto theirMinZ = aCollidedGameObject->GetMin().z;
	auto theirMaxZ = aCollidedGameObject->GetMax().z;

	bool minXtoMaxX = thisMinX <= theirMaxX;
	bool maxXtoMinX = thisMaxX >= theirMinX;
	bool minZtoMaxZ = thisMinZ <= theirMaxZ;
	bool maxZtoMinZ = thisMaxZ >= theirMinZ;

	bool collided = (minXtoMaxX && maxXtoMinX) && (minZtoMaxZ && maxZtoMinZ);

	return collided;

#else
	return (this->myMin.x <= aCollidedGameObject->GetMax().x && this->myMax.x >= aCollidedGameObject->GetMin().x) &&
		(this->myMin.z <= aCollidedGameObject->GetMax().z && this->myMax.z >= aCollidedGameObject->GetMin().z);
#endif
}

bool CRectangleColliderComponent::Collided(CCollider* aCollidedGameObject)
{
	return aCollidedGameObject->Collided(this);
}

void CRectangleColliderComponent::OnEnable()
{
#ifdef _DEBUG
	if (myVisualizer)
	{
		myVisualizer->SetIsActive(true);
	}
#endif // _DEBUG
}

void CRectangleColliderComponent::OnDisable()
{
#ifdef _DEBUG
	if (myVisualizer)
	{
		myVisualizer->SetPosition(CLineInstance::ourInactivePos);
		myVisualizer->SetIsActive(false);
	}
#endif // _DEBUG
}

float const CRectangleColliderComponent::GetHeight() const
{
	return myHeight;
}

void CRectangleColliderComponent::SetHeight(float aHeight)
{
	myHeight = aHeight;
}

float const CRectangleColliderComponent::GetWidth() const
{
	return myWidth;
}

void CRectangleColliderComponent::SetWidth(float aWidth)
{
	myWidth = aWidth;
}
