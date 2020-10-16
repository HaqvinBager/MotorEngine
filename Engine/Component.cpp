#include "stdafx.h"
#include "Component.h"
#include "GameObject.h"

CComponent::CComponent(CGameObject& aParent)
	: myParent(aParent),
	myEnabled(true)
{}

CComponent::~CComponent()
{
}

void CComponent::Collided(CGameObject* /*aCollidedGameObject*/)
{

}

CGameObject& CComponent::GetParent() const
{
	return myParent;
}
