#include "stdafx.h"
#include "AuraBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"

CAuraBehavior::CAuraBehavior(CGameObject* anObjectToFollow, float aRotationalSpeed)
	: myObjectToFollow(anObjectToFollow), myRotationalSpeed(aRotationalSpeed)
{
	myDuration = 600.0f;
	myTimer = 0.0f;
}

CAuraBehavior::~CAuraBehavior()
{
}

void CAuraBehavior::Update(CGameObject* aParent)
{
	//Don't update timer for permanent buff
	//myTimer += CTimer::Dt();
	//if (myTimer > myDuration) {
	//	myTimer = 0.0f;
	//	aParent->Active(false);
	//}
	aParent->GetComponent<CTransformComponent>()->Position(myObjectToFollow->myTransform->Position());
	DirectX::SimpleMath::Vector3 rotation = { 0.0f, myRotationalSpeed * CTimer::Dt(), 0.0f };
	aParent->GetComponent<CTransformComponent>()->Rotate(rotation);
}

void CAuraBehavior::Init(CGameObject* /*aCaster*/)
{
}
