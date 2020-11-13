#include "stdafx.h"
#include "Collider.h"

CCollider::CCollider(CGameObject& aParent, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags) : CBehaviour(aParent), myCollisionLayer(aCollisionLayer), myCollisionFlags(someCollisionFlags)
{
}

void CCollider::OnEnable()
{
}

void CCollider::OnDisable()
{
}

const ECollisionLayer CCollider::GetCollisionLayer() const
{
	return myCollisionLayer;
}

void CCollider::SetCollisionLayer(ECollisionLayer aCollisionLayer)
{
	myCollisionLayer = aCollisionLayer;
}

const DirectX::SimpleMath::Vector3 CCollider::GetPosition() const
{
	return myPosition;
}

void CCollider::SetPosition(DirectX::SimpleMath::Vector3 aPosition)
{
	myPosition = aPosition;
}

const uint64_t CCollider::GetCollidesWith() const
{
	return myCollisionFlags;
}

void CCollider::SetCollidesWith(uint64_t someCollisionFlags)
{
	myCollisionFlags = someCollisionFlags;
}
