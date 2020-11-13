#include "stdafx.h"
#include "Collider.h"

void CCollider::OnEnable()
{
}

void CCollider::OnDisable()
{
}

void CCollider::SetCollisionLayer(ECollisionLayer aCollisionLayer)
{
	myCollisionLayer = aCollisionLayer;
}

void CCollider::SetPosition(DirectX::SimpleMath::Vector3 aPosition)
{
	myPosition = aPosition;
}
