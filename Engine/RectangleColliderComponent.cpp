#include "stdafx.h"
#include "RectangleColliderComponent.h"
#include "CollisionManager.h"
#include "TransformComponent.h"

CRectangleColliderComponent::CRectangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight, bool isStatic) :
	CBehaviour(aParent),
	myWidth(aWidth),
	myHeight(aHeight),
	myIsStatic(isStatic)
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
}

CRectangleColliderComponent::~CRectangleColliderComponent() {
}

void CRectangleColliderComponent::Awake() {
	//TODO Fix after Merge Conflicts Resolved

	/*DirectX::SimpleMath::Vector3 vector = GameObject().GetComponent<CTransformComponent>()->Position() + GameObject().GetComponent<CTransformComponent>()->Position().Forward * (myHeight / 2.0f);

	myPosition = GameObject().GetComponent<CTransformComponent>()->Position();
	myVertices.emplace_back(vector - GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
	myVertices.emplace_back(vector + GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
	
	vector = GameObject().GetComponent<CTransformComponent>()->Position() - GameObject().GetComponent<CTransformComponent>()->Position().Forward * (myHeight / 2.0f);

	myVertices.emplace_back(vector - GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
	myVertices.emplace_back(vector + GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));*/
}

void CRectangleColliderComponent::Start() {
}

#include "Debug.h"
void CRectangleColliderComponent::Update() {
	
	
	if (!myIsStatic) {


		DirectX::SimpleMath::Vector3 vector = GameObject().GetComponent<CTransformComponent>()->Position() + GameObject().GetComponent<CTransformComponent>()->Position().Forward * (myHeight / 2.0f);
		myPosition = GameObject().GetComponent<CTransformComponent>()->Position();
		//TODO Fix after Merge Conflicts Resolved
		//myVertices[0] = vector - GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
		//myVertices[1] = vector + GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);

		vector = GameObject().GetComponent<CTransformComponent>()->Position() - GameObject().GetComponent<CTransformComponent>()->Position().Forward * (myHeight / 2.0f);

		//TODO Fix after Merge Conflicts Resolved
		//myVertices[2] = vector - GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
		//myVertices[3] = vector + GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
	}

	if (GetAsyncKeyState('C')) {
		CDebug::GetInstance()->DrawLine(myMin, myMax);
	}
}

void CRectangleColliderComponent::Collided(CGameObject* /*aCollidedGameObject*/) {
}

void CRectangleColliderComponent::OnEnable()
{
}

void CRectangleColliderComponent::OnDisable()
{
}
